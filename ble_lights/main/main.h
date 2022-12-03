#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GATTS_TABLE_TAG             "BLE_STATUS ::"
#define NUMBER_OF_PROFILES          1
#define BLE_PROFILE_APP_IDX         0
#define ESP_BLE_APP_ID              0x55
#define BLE_DEVICE_NAME             "VYBE"
#define BLE_SVC_INST_ID             0
#define ADV_CONFIG_FLAG             (1 << 0)
#define SCAN_RSP_CONFIG_FLAG        (1 << 1)
#define WDT_TIMEOUT                 120
#define HRPS_HT_MEAS_MAX_LEN        (13)

#define CONN_SUCCESS_STATUS     "ACK,CONN_SUCCESS"
#define ASK_TELEMETRY_TO_APP    "REQ,TO_APP_1"
#define ACK_CANCELLATION        "CANCEL"

#define FIRST_CONN_INITIATED                    1
#define FIRST_CONN_DATA_EXCHANGE_COMPLETE       0

#define BRIGHTNESS_LEVEL        "BRIGHTNESS,78"
#define RECEIVE_SUCCESSFUL      "ACK,SUCCESS"

#define LAMP_OFF                     "0"
#define PATTERN_1                    "1"
#define PATTERN_2                    "2"
#define PATTERN_3                    "3"


#define IDLE                    1
#define BUSY                    0
#define complete_data           "DATA_COMPLETE"

#define CHAR_DECLARATION_SIZE   (sizeof(uint8_t))

// SERVICES AND CHARACTERISTICS
static const uint8_t VEHICLE_SERVICE_UUID[16]                      = {0x10, 0xd8, 0x3e, 0x12, 0x54, 0x1f, 0x94, 0xad, 0x6c, 0x4b, 0x92, 0x81, 0x22, 0x35, 0xc8, 0x2c};

static const uint8_t ACK_CHARACTERISTIC_UUID[16]                   = {0x11, 0xd8, 0x3e, 0x12, 0x54, 0x1f, 0x94, 0xad, 0x6c, 0x4b, 0x92, 0x81, 0x22, 0x35, 0xc8, 0x2c};

static const uint8_t RGB_CHARACTERISTIC_UUID[16]                   = {0x12, 0xd8, 0x3e, 0x12, 0x54, 0x1f, 0x94, 0xad, 0x6c, 0x4b, 0x92, 0x81, 0x22, 0x35, 0xc8, 0x2c};
static const uint8_t BRIGHTNESS_CHARACTERISTIC_UUID[16]            = {0x13, 0xd8, 0x3e, 0x12, 0x54, 0x1f, 0x94, 0xad, 0x6c, 0x4b, 0x92, 0x81, 0x22, 0x35, 0xc8, 0x2c};
static const uint8_t MODE_CHARACTERISTIC_UUID[16]                  = {0x14, 0xd8, 0x3e, 0x12, 0x54, 0x1f, 0x94, 0xad, 0x6c, 0x4b, 0x92, 0x81, 0x22, 0x35, 0xc8, 0x2c};

int number_of_chunks_overall_data;
int number_of_chunks_suste_data;
int number_of_chunks_thikka_data;
int number_of_chunks_babbal_data;

int BLE_STATE;                      // This state will be BUSY when COMM is sending/receiving BLE data via VCU
char BLE_write_value[1];
int pairing_status;
int BLE_CONNECTION_STATUS;

int red_colorspace;
int green_colorspace;
int blue_colorspace;
int brightness_value;
int mode;
int ack_value;

typedef struct
{
    uint8_t *prepare_buf;
    int prepare_len;
} prepare_type_env_t;

// Attributes State Machine
enum
{
    main_service,

    rgb_characteristics_index,
    rgb_characteristics_value,
    rgb_characteristics_descriptor,
    
    brightness_characteristics_index,
    brightness_characteristics_value,
    brightness_characteristics_descriptor,
    
    ack_characteristics_index,
    ack_characteristics_value,
    ack_characteristics_descriptor,
    
    mode_characteristics_index,
    mode_characteristics_value,
    mode_characteristics_descriptor,
    
    HRS_IDX_NB,
};

// -------------------- DUMMY DATA FOR TEST -------------------- //
#define dummy_SoC_estRange_data            "46,58"
#define dummy_SoC_estRange_data1           "90,14"
#define dummy_eta_ChargingStatus_data      "57,1"
#define dummy_trip_telemetry_data          "22,33,44"

// #define dummy_overall_data1                "C1,111,222,333,444"
// #define dummy_overall_data2                "C2,555,666,777,888"
// #define dummy_overall_data3                "C3,999,000,111,222"

// #define dummy_suste_data1                  "C1,111,222,333,444"
// #define dummy_suste_data2                  "C2,555,666,777,888"
// #define dummy_suste_data3                  "C3,999,000,111,222"
// -------------------- DUMMY DATA FOR TEST -------------------- //