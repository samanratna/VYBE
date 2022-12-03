#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_bt.h"
#include "esp_task_wdt.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"
#include "main.h"

// Libraries from SIM Connectivity Code
#include "string.h"
#include "sim7600.h"
// #include "driver/twai.h"

// char* dummy_overall_data[]          = {"C1,111,111,111,111", "C2,222,222,222,222", "C3,333,333,333,333"};
// char* dummy_overall_data1[]         = {"C1,111,111,111,222", "C2,222,222,222,333", "C3,333,333,333,444"};
// char* dummy_suste_data[]            = {"C1,111,111,111,111", "C2,222,222,222,222", "C3,333,333,333,333"};
// char* dummy_thikka_data[]           = {"C1,111,111,111,111", "C2,222,222,222,222", "C3,333,333,333,333"};
// char* dummy_babbal_data[]           = {"C1,111,111,111,111", "C2,222,222,222,222", "C3,333,333,333,333"};

uint8_t notify_data[30];
static prepare_type_env_t prepare_write_env;
static uint8_t adv_config_done = 0;
static uint16_t BLE_COMM_DATABASE_TABLE[HRS_IDX_NB];
static uint8_t test_manufacturer[10] = {'E', 'S', 'P','3','2','A','B','C','D','E'};

// UUID 
static uint8_t sec_service_uuid[16] = {
    /* LSB <--------------------------------------------------------------------------------> MSB */
    // first uuid, 16bit, [12],[13] is the value
    0xfb,
    0x34,
    0x9b,
    0x5f,
    0x80,
    0x00,
    0x00,
    0x80,
    0x00,
    0x10,
    0x00,
    0x00,
    0x18,
    0x0E,
    0x00,
    0x00,
};

// config adv data
static esp_ble_adv_data_t ble_adv_config = {
    .set_scan_rsp = false,
    .include_txpower = true,
    .min_interval = 0x0006,                 // slave connection min interval, Time = min_interval * 1.25 msec
    .max_interval = 0x0010,                 // slave connection max interval, Time = max_interval * 1.25 msec
    .appearance = 0x00,
    .manufacturer_len = 0,                  // TEST_MANUFACTURER_DATA_LEN,
    .p_manufacturer_data = NULL,            // &test_manufacturer[0],
    .service_data_len = 0,
    .p_service_data = NULL,
    .service_uuid_len = sizeof(sec_service_uuid),
    .p_service_uuid = sec_service_uuid,
    .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
};

// config scan response data
static esp_ble_adv_data_t ble_rsp_config = {
    .set_scan_rsp = true,
    .include_name = true,
    .manufacturer_len = sizeof(test_manufacturer),
    .p_manufacturer_data = test_manufacturer,
};

static esp_ble_adv_params_t ble_adv_params = {
    .adv_int_min = 0x100,
    .adv_int_max = 0x100,
    .adv_type = ADV_TYPE_IND,
    .own_addr_type = BLE_ADDR_TYPE_RANDOM,
    .channel_map = ADV_CHNL_ALL,
    .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};

struct gatts_profile_inst
{
    esp_gatts_cb_t gatts_cb;
    uint16_t gatts_if;
    uint16_t app_id;
    uint16_t conn_id;
    uint16_t service_handle;
    esp_gatt_srvc_id_t service_id;
    uint16_t char_handle;
    esp_bt_uuid_t char_uuid;
    esp_gatt_perm_t perm;
    esp_gatt_char_prop_t property;
    uint16_t descr_handle;
    esp_bt_uuid_t descr_uuid;
};

// Function Prototypes
static void gatts_profile_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);
void BLE_notify(uint8_t *notify_data, uint8_t value_1, uint8_t value_2, int characteristic_value);
void BLE_notify_large(uint8_t *notify_data, uint32_t value_1, uint32_t value_2, int characteristic_value);
void BLE_notify_data(char *value, int characterictic_value);
void Convert_Text2Hex_username(char *name, char *current_name);
void Convert_Text2Hex_bikename(char *name, char *current_name);
void CAN_send_username(void);
void CAN_send_bikename(void);
void sync_app_features(void);
void BLE_notify_app_features(void);

/* One gatt-based profile one app_id and one gatts_if, this array will store the gatts_if returned by ESP_GATTS_REG_EVT */
static struct gatts_profile_inst ble_profile_tab[NUMBER_OF_PROFILES] = {
    [BLE_PROFILE_APP_IDX] = {
        .gatts_cb = gatts_profile_event_handler,
        .gatts_if = ESP_GATT_IF_NONE, /* Not get the gatt_if, so initial is ESP_GATT_IF_NONE */
    },

};



static const uint16_t primary_service_uuid              = ESP_GATT_UUID_PRI_SERVICE;
static const uint16_t character_declaration_uuid        = ESP_GATT_UUID_CHAR_DECLARE;
static const uint16_t character_client_config_uuid      = ESP_GATT_UUID_CHAR_CLIENT_CONFIG;
static const uint8_t char_prop_read_write_notify        = ESP_GATT_CHAR_PROP_BIT_READ | ESP_GATT_CHAR_PROP_BIT_WRITE | ESP_GATT_CHAR_PROP_BIT_NOTIFY;
static const uint8_t ble_measurement_cc[2]              = {0x00, 0x00};

// Full HRS Database Description - Used to add attributes into the database
static const esp_gatts_attr_db_t ble_gatt_db[HRS_IDX_NB] = {

    [main_service]                                 = {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&primary_service_uuid, ESP_GATT_PERM_READ, sizeof(uint16_t), sizeof(VEHICLE_SERVICE_UUID), (uint8_t *)&VEHICLE_SERVICE_UUID}},

    [ack_characteristics_index]                    = {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&character_declaration_uuid, ESP_GATT_PERM_READ, CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE, (uint8_t *)&char_prop_read_write_notify}},
    [ack_characteristics_value]                    = {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_128, (uint8_t *)&ACK_CHARACTERISTIC_UUID, ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, HRPS_HT_MEAS_MAX_LEN, 0, NULL}},
    [ack_characteristics_descriptor]               = {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&character_client_config_uuid, ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, sizeof(uint16_t), sizeof(ble_measurement_cc), (uint8_t *)ble_measurement_cc}},
    
    [rgb_characteristics_index]                    = {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&character_declaration_uuid, ESP_GATT_PERM_READ, CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE, (uint8_t *)&char_prop_read_write_notify}},
    [rgb_characteristics_value]                    = {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_128, (uint8_t *)&RGB_CHARACTERISTIC_UUID, ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, HRPS_HT_MEAS_MAX_LEN, 0, NULL}},
    [rgb_characteristics_descriptor]               = {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&character_client_config_uuid, ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, sizeof(uint16_t), sizeof(ble_measurement_cc), (uint8_t *)ble_measurement_cc}},

    [brightness_characteristics_index]             = {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&character_declaration_uuid, ESP_GATT_PERM_READ, CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE, (uint8_t *)&char_prop_read_write_notify}},
    [brightness_characteristics_value]             = {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_128, (uint8_t *)&BRIGHTNESS_CHARACTERISTIC_UUID, ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, HRPS_HT_MEAS_MAX_LEN, 0, NULL}},
    [brightness_characteristics_descriptor]        = {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&character_client_config_uuid, ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, sizeof(uint16_t), sizeof(ble_measurement_cc), (uint8_t *)ble_measurement_cc}},

    [mode_characteristics_index]                   = {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&character_declaration_uuid, ESP_GATT_PERM_READ, CHAR_DECLARATION_SIZE, CHAR_DECLARATION_SIZE, (uint8_t *)&char_prop_read_write_notify}},
    [mode_characteristics_value]                   = {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_128, (uint8_t *)&MODE_CHARACTERISTIC_UUID, ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, HRPS_HT_MEAS_MAX_LEN, 0, NULL}},
    [mode_characteristics_descriptor]              = {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&character_client_config_uuid, ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, sizeof(uint16_t), sizeof(ble_measurement_cc), (uint8_t *)ble_measurement_cc}},
};

static char *esp_key_type_to_str(esp_ble_key_type_t key_type)
{
    char *key_str = NULL;
    switch (key_type)
    {
    case ESP_LE_KEY_NONE:
        key_str = "ESP_LE_KEY_NONE";
        break;
    case ESP_LE_KEY_PENC:
        key_str = "ESP_LE_KEY_PENC";
        break;
    case ESP_LE_KEY_PID:
        key_str = "ESP_LE_KEY_PID";
        break;
    case ESP_LE_KEY_PCSRK:
        key_str = "ESP_LE_KEY_PCSRK";
        break;
    case ESP_LE_KEY_PLK:
        key_str = "ESP_LE_KEY_PLK";
        break;
    case ESP_LE_KEY_LLK:
        key_str = "ESP_LE_KEY_LLK";
        break;
    case ESP_LE_KEY_LENC:
        key_str = "ESP_LE_KEY_LENC";
        break;
    case ESP_LE_KEY_LID:
        key_str = "ESP_LE_KEY_LID";
        break;
    case ESP_LE_KEY_LCSRK:
        key_str = "ESP_LE_KEY_LCSRK";
        break;
    default:
        key_str = "INVALID BLE KEY TYPE";
        break;
    }

    return key_str;
}

static char *esp_auth_req_to_str(esp_ble_auth_req_t auth_req)
{
    char *auth_str = NULL;
    switch (auth_req)
    {
    case ESP_LE_AUTH_NO_BOND:
        auth_str = "ESP_LE_AUTH_NO_BOND";
        break;
    case ESP_LE_AUTH_BOND:
        auth_str = "ESP_LE_AUTH_BOND";
        break;
    case ESP_LE_AUTH_REQ_MITM:
        auth_str = "ESP_LE_AUTH_REQ_MITM";
        break;
    case ESP_LE_AUTH_REQ_BOND_MITM:
        auth_str = "ESP_LE_AUTH_REQ_BOND_MITM";
        break;
    case ESP_LE_AUTH_REQ_SC_ONLY:
        auth_str = "ESP_LE_AUTH_REQ_SC_ONLY";
        break;
    case ESP_LE_AUTH_REQ_SC_BOND:
        auth_str = "ESP_LE_AUTH_REQ_SC_BOND";
        break;
    case ESP_LE_AUTH_REQ_SC_MITM:
        auth_str = "ESP_LE_AUTH_REQ_SC_MITM";
        break;
    case ESP_LE_AUTH_REQ_SC_MITM_BOND:
        auth_str = "ESP_LE_AUTH_REQ_SC_MITM_BOND";
        break;
    default:
        auth_str = "INVALID BLE AUTH REQ";
        break;
    }

    return auth_str;
}

static void show_bonded_devices(void)
{
    int dev_num = esp_ble_get_bond_device_num();

    esp_ble_bond_dev_t *dev_list = (esp_ble_bond_dev_t *)malloc(sizeof(esp_ble_bond_dev_t) * dev_num);
    esp_ble_get_bond_device_list(&dev_num, dev_list);
    ESP_LOGI(GATTS_TABLE_TAG, "Bonded devices number : %d\n", dev_num);

    ESP_LOGI(GATTS_TABLE_TAG, "Bonded devices list : %d\n", dev_num);
    for (int i = 0; i < dev_num; i++)
    {
        esp_log_buffer_hex(GATTS_TABLE_TAG, (void *)dev_list[i].bd_addr, sizeof(esp_bd_addr_t));
    }
    free(dev_list);
}

static void __attribute__((unused)) remove_all_bonded_devices(void)
{
    int dev_num = esp_ble_get_bond_device_num();

    esp_ble_bond_dev_t *dev_list = (esp_ble_bond_dev_t *)malloc(sizeof(esp_ble_bond_dev_t) * dev_num);
    esp_ble_get_bond_device_list(&dev_num, dev_list);
    for (int i = 0; i < dev_num; i++)
    {
        esp_ble_remove_bond_device(dev_list[i].bd_addr);
    }
    free(dev_list);
}

static void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{
    ESP_LOGV(GATTS_TABLE_TAG, "GAP_EVT, event %d\n", event);

    switch (event)
    {

    case ESP_GAP_BLE_SCAN_RSP_DATA_SET_COMPLETE_EVT:

        ESP_LOGI(GATTS_TABLE_TAG, "ESP_GATTS_EXEC_WRITE_EVT");
        adv_config_done &= (~SCAN_RSP_CONFIG_FLAG);
        if (adv_config_done == 0)
        {
            esp_ble_gap_start_advertising(&ble_adv_params);
        }
        break;

    case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:

        ESP_LOGI(GATTS_TABLE_TAG, "ESP_GATTS_EXEC_WRITE_EVT");
        adv_config_done &= (~ADV_CONFIG_FLAG);
        if (adv_config_done == 0)
        {
            esp_ble_gap_start_advertising(&ble_adv_params);
        }
        break;

    case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:

        ESP_LOGI(GATTS_TABLE_TAG, "ESP_GATTS_EXEC_WRITE_EVT");
        // advertising start complete event to indicate advertising start successfully or failed
        if (param->adv_start_cmpl.status != ESP_BT_STATUS_SUCCESS)
        {
            ESP_LOGE(GATTS_TABLE_TAG, "advertising start failed, error status = %x", param->adv_start_cmpl.status);
            break;
        }
        ESP_LOGI(GATTS_TABLE_TAG, "advertising start success");
        break;

    case ESP_GAP_BLE_PASSKEY_REQ_EVT:

        /* passkey request event */
        ESP_LOGI(GATTS_TABLE_TAG, "ESP_GAP_BLE_PASSKEY_REQ_EVT");
        /* Call the following function to input the passkey which is displayed on the remote device */
        // esp_ble_passkey_reply(ble_profile_tab[BLE_PROFILE_APP_IDX].remote_bda, true, 0x00);
        break;

    case ESP_GAP_BLE_OOB_REQ_EVT:

        ESP_LOGI(GATTS_TABLE_TAG, "ESP_GAP_BLE_OOB_REQ_EVT");
        uint8_t tk[16] = {1}; // If you paired with OOB, both devices need to use the same tk
        esp_ble_oob_req_reply(param->ble_security.ble_req.bd_addr, tk, sizeof(tk));
        break;
    
    case ESP_GAP_BLE_LOCAL_IR_EVT:

        /* BLE local IR event */
        ESP_LOGI(GATTS_TABLE_TAG, "ESP_GAP_BLE_LOCAL_IR_EVT");
        break;

    case ESP_GAP_BLE_LOCAL_ER_EVT:

        /* BLE local ER event */
        ESP_LOGI(GATTS_TABLE_TAG, "ESP_GAP_BLE_LOCAL_ER_EVT");
        break;

    case ESP_GAP_BLE_NC_REQ_EVT:

        /* The app will receive this evt when the IO has DisplayYesNO capability and the peer device IO also has DisplayYesNo capability.
        show the passkey number to the user to confirm it with the number displayed by peer device. */
        esp_ble_confirm_reply(param->ble_security.ble_req.bd_addr, true);
        ESP_LOGI(GATTS_TABLE_TAG, "ESP_GAP_BLE_NC_REQ_EVT, the passkey Notify number:%d", param->ble_security.key_notif.passkey);
        break;

    case ESP_GAP_BLE_SEC_REQ_EVT:

        /* send the positive(true) security response to the peer device to accept the security request.
        If not accept the security request, should send the security response with negative(false) accept value*/
        esp_ble_gap_security_rsp(param->ble_security.ble_req.bd_addr, true);
        break;

    case ESP_GAP_BLE_PASSKEY_NOTIF_EVT:

        /// the app will receive this evt when the IO  has Output capability and the peer device IO has Input capability.
        /// show the passkey number to the user to input it in the peer device.
        ESP_LOGI(GATTS_TABLE_TAG, "The passkey Notify number:%06d", param->ble_security.key_notif.passkey);

        // Send Passkey to CAN to be displayed on dashboard
        int _passkey = param->ble_security.key_notif.passkey;
        printf("BLE_STATUS :: YOUR PASSKEY IS %d\n", _passkey);
        uint8_t passkey_array[6];
        for (int i = 0; i < 6; i++)
        {
            passkey_array[i] = _passkey % 10;
            _passkey = _passkey / 10;
            // printf("passkeys: %d\n", passkey_array[i]);
        }

        // Send all Passkeys to CAN
        twai_tx_data[0] = passkey_array[5];
        twai_tx_data[1] = passkey_array[4];
        twai_tx_data[2] = passkey_array[3];
        twai_tx_data[3] = passkey_array[2];
        twai_tx_data[4] = passkey_array[1];
        twai_tx_data[5] = passkey_array[0];
        twai_tx(0x18FF0E28, 8);
        twai_tx_data[0] = 0;
        twai_tx_data[1] = 0;
        twai_tx_data[2] = 0;
        twai_tx_data[3] = 0;
        twai_tx_data[4] = 0;
        twai_tx_data[5] = 0;

        break;

    case ESP_GAP_BLE_KEY_EVT:

        // shows the ble key info share with peer device to the user.
        ESP_LOGI(GATTS_TABLE_TAG, "key type = %s", esp_key_type_to_str(param->ble_security.ble_key.key_type));
        break;

    case ESP_GAP_BLE_AUTH_CMPL_EVT:
    {

        esp_bd_addr_t bd_addr;
        memcpy(bd_addr, param->ble_security.auth_cmpl.bd_addr, sizeof(esp_bd_addr_t));
        ESP_LOGI(GATTS_TABLE_TAG, "remote BD_ADDR: %08x%04x",
                (bd_addr[0] << 24) + (bd_addr[1] << 16) + (bd_addr[2] << 8) + bd_addr[3],
                (bd_addr[4] << 8) + bd_addr[5]);
        ESP_LOGI(GATTS_TABLE_TAG, "address type = %d", param->ble_security.auth_cmpl.addr_type);
        ESP_LOGI(GATTS_TABLE_TAG, "pair status = %s", param->ble_security.auth_cmpl.success ? "success" : "fail");
        
        if (!param->ble_security.auth_cmpl.success)
        {
            ESP_LOGI(GATTS_TABLE_TAG, "fail reason = 0x%x", param->ble_security.auth_cmpl.fail_reason);

            // Pairing Unsuccessful
            printf("BLE_STATUS :: PAIRING FAILED : CAN'T SEND CAN REQUEST \n");

            // Disconnect on Connection unsuccessful
            esp_ble_gatts_close(3, 0);
            pairing_status = 0;
        }
        else
        {
            ESP_LOGI(GATTS_TABLE_TAG, "auth mode = %s", esp_auth_req_to_str(param->ble_security.auth_cmpl.auth_mode));

            // Pairing Successful
            printf("BLE_STATUS :: PAIRING SUCCESSFUL\n");

            // Notify pairing success to YatriHub
            vTaskDelay(3000 / portTICK_PERIOD_MS);
            BLE_notify_data(CONN_SUCCESS_STATUS, ack_characteristics_value);
            pairing_status = 1;
        }
        // show_bonded_devices();
        break;
    }

    case ESP_GAP_BLE_REMOVE_BOND_DEV_COMPLETE_EVT:
    {

        ESP_LOGD(GATTS_TABLE_TAG, "ESP_GAP_BLE_REMOVE_BOND_DEV_COMPLETE_EVT status = %d", param->remove_bond_dev_cmpl.status);
        ESP_LOGI(GATTS_TABLE_TAG, "ESP_GAP_BLE_REMOVE_BOND_DEV");
        ESP_LOGI(GATTS_TABLE_TAG, "-----ESP_GAP_BLE_REMOVE_BOND_DEV----");
        esp_log_buffer_hex(GATTS_TABLE_TAG, (void *)param->remove_bond_dev_cmpl.bd_addr, sizeof(esp_bd_addr_t));
        ESP_LOGI(GATTS_TABLE_TAG, "------------------------------------");
        break;
    }

    case ESP_GAP_BLE_SET_LOCAL_PRIVACY_COMPLETE_EVT:

        if (param->local_privacy_cmpl.status != ESP_BT_STATUS_SUCCESS)
        {
            ESP_LOGE(GATTS_TABLE_TAG, "config local privacy failed, error status = %x", param->local_privacy_cmpl.status);
            break;
        }

        esp_err_t ret = esp_ble_gap_config_adv_data(&ble_adv_config);
        if (ret)
        {
            ESP_LOGE(GATTS_TABLE_TAG, "config adv data failed, error code = %x", ret);
        }
        else
        {
            adv_config_done |= ADV_CONFIG_FLAG;
        }

        ret = esp_ble_gap_config_adv_data(&ble_rsp_config);
        if (ret)
        {
            ESP_LOGE(GATTS_TABLE_TAG, "config adv data failed, error code = %x", ret);
        }
        else
        {
            adv_config_done |= SCAN_RSP_CONFIG_FLAG;
        }

        break;
    default:
        break;
    }
}

void example_exec_write_event_env(prepare_type_env_t *prepare_write_env, esp_ble_gatts_cb_param_t *param)
{
    ESP_LOGI(GATTS_TABLE_TAG, "ESP_GATTS_EXEC_WRITE_EVT");

    if (param->exec_write.exec_write_flag == ESP_GATT_PREP_WRITE_EXEC && prepare_write_env->prepare_buf){
        esp_log_buffer_hex(GATTS_TABLE_TAG, prepare_write_env->prepare_buf, prepare_write_env->prepare_len);
    } else {
        ESP_LOGI(GATTS_TABLE_TAG, "ESP_GATT_PREP_WRITE_CANCEL");
    }

    if (prepare_write_env->prepare_buf){
        free(prepare_write_env->prepare_buf);
        prepare_write_env->prepare_buf = NULL;
    }
    prepare_write_env->prepare_len = 0;
}

void BLE_notify_data(char *value, int characteristic_value){
    printf("BLE_STATUS :: Notifying the following data :: %s\n", value);
    esp_ble_gatts_send_indicate(3, 0, BLE_COMM_DATABASE_TABLE[characteristic_value], strlen(value), (uint8_t *)value, false);
}

static void gatts_profile_event_handler(esp_gatts_cb_event_t event,
                                        esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)
{

    ESP_LOGV(GATTS_TABLE_TAG, "event = %x\n", event);
    switch (event)
    {

        // EVENT : WHEN BLE DRIVERS ARE ALL CONFIGURED
        case ESP_GATTS_REG_EVT:
            ESP_LOGI(GATTS_TABLE_TAG, "ESP_GATTS_REG_EVT");
            esp_ble_gap_set_device_name(BLE_DEVICE_NAME);
            // generate a resolvable random address
            esp_ble_gap_config_local_privacy(true);
            esp_ble_gatts_create_attr_tab(ble_gatt_db, gatts_if, HRS_IDX_NB, BLE_SVC_INST_ID);
            break;

        case ESP_GATTS_READ_EVT:
            ESP_LOGI(GATTS_TABLE_TAG, "ESP_GATTS_READ_EVT");
            break;

        case ESP_GATTS_WRITE_EVT:

            if (!param->write.is_prep)
            {
                if (pairing_status == 1)
                {
                    // ESP_LOGI(GATTS_TABLE_TAG, "ESP_GATTS_WRITE_EVT, write value:");
                    // esp_log_buffer_hex(GATTS_TABLE_TAG, param->write.value, param->write.len);

                    // Write Operation
                    // printf("WRITE VALUE RECEIVED : %s \n", param->write.value);
                    memset(BLE_write_value, 0, sizeof(BLE_write_value));
                    sprintf(BLE_write_value, "%s", param->write.value);

                    // RECEIVE ACK STATUS
                    if(BLE_COMM_DATABASE_TABLE[ack_characteristics_value]==param->write.handle){

                        ack_value    = atoi(BLE_write_value);
                        printf("ACKNOWLEDGEMENT VALUE:: %d\n", ack_value);

                        // ACK FOR SUCCESSFUL DATA RETRIEVE
                        BLE_notify_data(RECEIVE_SUCCESSFUL, ack_characteristics_value);
                    }

                    // RECEIVE RGB COLORSPACE VALUE
                    if(BLE_COMM_DATABASE_TABLE[rgb_characteristics_value]==param->write.handle){

                        char rx_buffer[30];
                        sprintf(rx_buffer, "%s", BLE_write_value);
                        char *data_array[30] = {0};
                        int i = 0;
                        char delimiter[] = ",";
                        char *ptr = strtok((char *)rx_buffer, delimiter);
                        while (ptr != NULL)
                        {
                            data_array[i] = ptr;
                            ptr = strtok(NULL, delimiter);
                            printf(data_array[i]);
                            printf("\n");
                            i++;
                        }

                        red_colorspace      = atoi(data_array[0]);
                        green_colorspace    = atoi(data_array[1]);
                        blue_colorspace     = atoi(data_array[2]);

                        printf("RED COLORSPACE      :: %d\n", red_colorspace);
                        printf("GREEN COLORSPACE    :: %d\n", green_colorspace);
                        printf("BLUE COLORSPACE     :: %d\n", blue_colorspace);

                        // ACK FOR SUCCESSFUL DATA RETRIEVE
                        BLE_notify_data(RECEIVE_SUCCESSFUL, rgb_characteristics_value);
                    }

                    // RECEIVE BRIGHTNESS VALUE
                    if(BLE_COMM_DATABASE_TABLE[brightness_characteristics_value]==param->write.handle){

                        brightness_value    = atoi(BLE_write_value);
                        printf("BRIGHTNESS VALUE    :: %d\n", brightness_value);

                        // ACK FOR SUCCESSFUL DATA RETRIEVE
                        BLE_notify_data(RECEIVE_SUCCESSFUL, brightness_characteristics_value);
                    }

                    // RECEIVE MODE VALUE
                    if(BLE_COMM_DATABASE_TABLE[mode_characteristics_value]==param->write.handle){

                        mode                = atoi(BLE_write_value);
                        printf("MODE                :: %d\n", mode);

                        // ACK FOR SUCCESSFUL DATA RETRIEVE
                        BLE_notify_data(RECEIVE_SUCCESSFUL, mode_characteristics_value);
                    }
                }
            }

            if (param->write.need_rsp)
            {
                printf("Reached Write Event...");
                esp_ble_gatts_send_response(gatts_if, param->write.conn_id, param->write.trans_id, ESP_GATT_OK, NULL);
            }
            break;
        case ESP_GATTS_EXEC_WRITE_EVT:
            ESP_LOGI(GATTS_TABLE_TAG, "ESP_GATTS_EXEC_WRITE_EVT");
            example_exec_write_event_env(&prepare_write_env, param);
            break;
        case ESP_GATTS_MTU_EVT:
            ESP_LOGI(GATTS_TABLE_TAG, "ESP_GATTS_MTU_EVT");
            break;
        case ESP_GATTS_CONF_EVT:
            ESP_LOGI(GATTS_TABLE_TAG, "ESP_GATTS_CONF_EVT");
            break;
        case ESP_GATTS_UNREG_EVT:
            break;
        case ESP_GATTS_DELETE_EVT:
            ESP_LOGI(GATTS_TABLE_TAG, "ESP_GATTS_DELETE_EVT");
            break;
        case ESP_GATTS_START_EVT:
            ESP_LOGI(GATTS_TABLE_TAG, "ESP_GATTS_START_EVT");
            esp_ble_gap_start_advertising(&ble_adv_params); // advertise once again
            break;
        case ESP_GATTS_STOP_EVT:
            ESP_LOGI(GATTS_TABLE_TAG, "ESP_GATTS_STOP_EVT");
            break;
        case ESP_GATTS_CONNECT_EVT:
            ESP_LOGI(GATTS_TABLE_TAG, "ESP_GATTS_CONNECT_EVT");
            ESP_LOGI(GATTS_TABLE_TAG, "BLE_STATUS :: Device is just connected!");
            /* start security connect with peer device when receive the connect event sent by the master */
            esp_ble_set_encryption(param->connect.remote_bda, ESP_BLE_SEC_ENCRYPT_MITM);
            break;
        case ESP_GATTS_DISCONNECT_EVT:
            // BLE_notify_data(ACK_CANCELLATION,ack_characteristics_value);
            ESP_LOGI(GATTS_TABLE_TAG, "ESP_GATTS_DISCONNECT_EVT, disconnect reason 0x%x", param->disconnect.reason);
            /* start advertising again when missing the connect */
            esp_ble_gap_start_advertising(&ble_adv_params);
            BLE_CONNECTION_STATUS = FIRST_CONN_DATA_EXCHANGE_COMPLETE;
            break;
        case ESP_GATTS_OPEN_EVT:
            ESP_LOGI(GATTS_TABLE_TAG, "ESP_GATTS_CONNECT_EVT from ESP_GATTS_OPEN_EVT\n");
            break;
        case ESP_GATTS_CANCEL_OPEN_EVT:
            ESP_LOGI(GATTS_TABLE_TAG, "ESP_GATTS_CONNECT_EVT from ESP_GATTS_CANCEL_OPEN_EVT\n");
            break;
        case ESP_GATTS_CLOSE_EVT:
            printf("BLE_STATUS :: DEVICE IS DISCONNECTED from ESP_GATTS_CLOSE_EVT\n");
            break;
        case ESP_GATTS_LISTEN_EVT:
            ESP_LOGI(GATTS_TABLE_TAG, "ESP_GATTS_CONNECT_EVT from ESP_GATTS_LISTEN_EVT\n");
            break;
        case ESP_GATTS_CONGEST_EVT:
            ESP_LOGI(GATTS_TABLE_TAG, "ESP_GATTS_CONNECT_EVT from ESP_GATTS_CONGEST_EVT\n");
            break;
        case ESP_GATTS_CREAT_ATTR_TAB_EVT:
        {
            ESP_LOGI(GATTS_TABLE_TAG, "The number handle = %x", param->add_attr_tab.num_handle);
            if (param->create.status == ESP_GATT_OK){
                if (param->add_attr_tab.num_handle == HRS_IDX_NB){
                    memcpy(BLE_COMM_DATABASE_TABLE, param->add_attr_tab.handles, sizeof(BLE_COMM_DATABASE_TABLE));
                    esp_ble_gatts_start_service(BLE_COMM_DATABASE_TABLE[main_service]);
                    printf("BLE_STATUS :: STARTED SERVICE HERE ::\n");
                } else {
                    ESP_LOGE(GATTS_TABLE_TAG, "Create attribute table abnormally, num_handle (%d) doesn't equal to HRS_IDX_NB(%d)",
                            param->add_attr_tab.num_handle, HRS_IDX_NB);
                }
            }
            else {
                ESP_LOGE(GATTS_TABLE_TAG, " Create attribute table failed, error code = %x", param->create.status);
            }
            break;
        }
        default:
            break;
        }
    }


static void gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if,
                                esp_ble_gatts_cb_param_t *param)
{
    /* If event is register event, store the gatts_if for each profile */
    if (event == ESP_GATTS_REG_EVT){
        if (param->reg.status == ESP_GATT_OK){
            ble_profile_tab[BLE_PROFILE_APP_IDX].gatts_if = gatts_if;
        } else {
            ESP_LOGI(GATTS_TABLE_TAG, "Reg app failed, app_id %04x, status %d\n",
                     param->reg.app_id,
                     param->reg.status);
            return;
        }
    }

    do {
        int idx;
        for (idx = 0; idx < NUMBER_OF_PROFILES; idx++){
            if (gatts_if == ESP_GATT_IF_NONE || /* ESP_GATT_IF_NONE, not specify a certain gatt_if, need to call every profile cb function */
                gatts_if == ble_profile_tab[idx].gatts_if) {
                if (ble_profile_tab[idx].gatts_cb){
                    ble_profile_tab[idx].gatts_cb(event, gatts_if, param);
                }
            }
        }
    } while (0);
}

void app_main(void)
{
    number_of_chunks_overall_data = 3;
    number_of_chunks_suste_data = 3;
    number_of_chunks_thikka_data = 3;
    number_of_chunks_babbal_data = 3;

    // Default values for App features and Username (On ESP32 Reset)
    strcpy(g_last_username, "Yatri");
    strcpy(g_last_bikename, "UN-NAMED");
    strcpy(gps_tracking, "K");
    strcpy(shake_mode, "l");
    strcpy(anti_theft, "m");

    // Initialize TWDT
    ESP_ERROR_CHECK(esp_task_wdt_init(WDT_TIMEOUT, false));

    esp_err_t ret;

    // Initialize NVS.
    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ret = esp_bt_controller_init(&bt_cfg);
    if (ret)
    {
        ESP_LOGE(GATTS_TABLE_TAG, "%s init controller failed: %s", __func__, esp_err_to_name(ret));
        return;
    }
    ret = esp_bt_controller_enable(ESP_BT_MODE_BLE);
    if (ret)
    {
        ESP_LOGE(GATTS_TABLE_TAG, "%s enable controller failed: %s", __func__, esp_err_to_name(ret));
        return;
    }

    ESP_LOGI(GATTS_TABLE_TAG, "%s init bluetooth", __func__);
    ret = esp_bluedroid_init();
    if (ret)
    {
        ESP_LOGE(GATTS_TABLE_TAG, "%s init bluetooth failed: %s", __func__, esp_err_to_name(ret));
        esp_restart();
        return;
    }

    ret = esp_bluedroid_enable();
    if (ret)
    {
        ESP_LOGE(GATTS_TABLE_TAG, "%s enable bluetooth failed: %s", __func__, esp_err_to_name(ret));
        return;
    }

    ret = esp_ble_gatts_register_callback(gatts_event_handler);
    if (ret)
    {
        ESP_LOGE(GATTS_TABLE_TAG, "gatts register error, error code = %x", ret);
        return;
    }
    ret = esp_ble_gap_register_callback(gap_event_handler);
    if (ret)
    {
        ESP_LOGE(GATTS_TABLE_TAG, "gap register error, error code = %x", ret);
        return;
    }
    ret = esp_ble_gatts_app_register(ESP_BLE_APP_ID);
    if (ret)
    {
        ESP_LOGE(GATTS_TABLE_TAG, "gatts app register error, error code = %x", ret);
        return;
    }

    /* set the security iocap & auth_req & key size & init key response key parameters to the stack*/
    esp_ble_auth_req_t auth_req = ESP_LE_AUTH_REQ_SC_MITM_BOND; // bonding with peer device after authentication
    esp_ble_io_cap_t iocap = ESP_IO_CAP_NONE;                    // set the IO capability to No output No input
    uint8_t key_size = 16;                                      // the key size should be 7~16 bytes
    uint8_t init_key = ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK;
    uint8_t rsp_key = ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK;

    // set static passkey
    uint32_t passkey = rand();
    uint8_t auth_option = ESP_BLE_ONLY_ACCEPT_SPECIFIED_AUTH_DISABLE;
    uint8_t oob_support = ESP_BLE_OOB_DISABLE;

    esp_ble_gap_set_security_param(ESP_BLE_SM_SET_STATIC_PASSKEY, &passkey, sizeof(uint32_t));
    esp_ble_gap_set_security_param(ESP_BLE_SM_AUTHEN_REQ_MODE, &auth_req, sizeof(uint8_t));
    esp_ble_gap_set_security_param(ESP_BLE_SM_IOCAP_MODE, &iocap, sizeof(uint8_t));
    esp_ble_gap_set_security_param(ESP_BLE_SM_MAX_KEY_SIZE, &key_size, sizeof(uint8_t));
    esp_ble_gap_set_security_param(ESP_BLE_SM_ONLY_ACCEPT_SPECIFIED_SEC_AUTH, &auth_option, sizeof(uint8_t));
    esp_ble_gap_set_security_param(ESP_BLE_SM_OOB_SUPPORT, &oob_support, sizeof(uint8_t));

    /* If your BLE device acts as a Slave, the init_key means you hope which types of key of the master should distribute to you,
    and the response key means which key you can distribute to the master;
    If your BLE device acts as a master, the response key means you hope which types of key of the slave should distribute to you,
    and the init key means which key you can distribute to the slave. */
    esp_ble_gap_set_security_param(ESP_BLE_SM_SET_INIT_KEY, &init_key, sizeof(uint8_t));
    esp_ble_gap_set_security_param(ESP_BLE_SM_SET_RSP_KEY, &rsp_key, sizeof(uint8_t));

    /* Just show how to clear all the bonded devices
     * Delay 30s, clear all the bonded devices
     *
     * vTaskDelay(30000 / portTICK_PERIOD_MS);
     * remove_all_bonded_devices();
     */

    // remove_all_bonded_devices();
    vTaskDelay(3000 / portTICK_PERIOD_MS);
}