#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "sdkconfig.h"
#include "string.h"
#include "sim7600.h"
#include "driver/twai.h"

const int uart_port0 = UART_NUM_0; 
const int uart_port2 = UART_NUM_2; 

#if 1
/*
 * Bike Credentials for Production Server
*/

char bikeVerifyUrl[] = "http://api.embedded.yatrimotorcycles.com/api/v1/charge-stations/payments/verify-user";
char ApiKey[] = "key:665ced149c081ce434f7e4aa2ad92d43fdf61d0704a4a6ea";
char crashReportUrl[] = "http://api.embedded.yatrimotorcycles.com/api/v1/bikes/crash";
char crashReportApiKey[] = "key:1c599aff61d7395ef020ac3e6f5777b3d9f7f521fb47ddf2";
char bikeLocationUrl[] = "http://api.embedded.yatrimotorcycles.com/api/v1/bikes/update-location";
char bikeLocationApiKey[] = "key:af8c5e643481fcb4388e336b5621306737998dde08e3fc91";

char mobileNumber[] = "+9779811093836";  // Enter the Mobile Number you want to send to (EMERGENCY CONTACT ONLY)

//for bike
char b_socUrl[] = "http://api.embedded.yatrimotorcycles.com/api/v1/bikes/batteries/soc";
char b_chargeCompleteUrl[] = "http://api.embedded.yatrimotorcycles.com/api/v1/bikes/charge-complete";
char b_chargerPluggedUrl[] = "http://api.embedded.yatrimotorcycles.com/api/v1/bikes/charge-plugged";
char b_chargerUnpluggedUrl[] = "http://api.embedded.yatrimotorcycles.com/api/v1/bikes/charge-unplugged";
char b_telemetryUrl[] = "http://api.embedded.yatrimotorcycles.com/api/v1/bikes/telemetries";
char b_theftAlertUrl[] = "http://api.embedded.yatrimotorcycles.com/api/v1/bikes/theft-alert";
char b_batteryLogUrl[] = "http://api.embedded.yatrimotorcycles.com/api/v1/bikes/battery-log";

// API Keys //
// char b_apiKey[] = "key:1df6bb339956c8d7eeb00116b9be0d707649c92f9b56ed90";     // Abhay dai
// char b_apiKey[] = "key:31e165db1e70a3d68691f85c7edfe8bbf1f6599769608669";     // Ashim dai
// char b_apiKey[] = "key:1c599aff61d7395ef020ac3e6f5777b3d9f7f521fb47ddf2";     // Saman
// char b_apiKey[] = "key:5199b4723e9c24a35421418eaacbf0d6ee714101456d34ee";     // YDS-041
// char b_apiKey[] = "key:5199b4723e9c24a35421418eaacbf0d6ee714101456d34ee";     // Cloud Burst
// char b_apiKey[] = "key:238c2a157e37f5af036edc9433615ef8c21289a038a82134";     // YDS-069 Kushal Singh
// char b_apiKey[] = "key:d8b9c9670c9717be1f8c74c9e0eb83ec82020fd791c8d0ac";      // Yatri-User
// char b_apiKey[] = "key:fcc9e854ce88f2724939d024780ddb998aafdf320c174938";     // Yatri-Support Cloud Burst
// char b_apiKey[] = "key:d8b9c9670c9717be1f8c74c9e0eb83ec82020fd791c8d0ac";     // user-test4 Sun Glow
// char b_apiKey[] = "key:bf11c8b93818262bac4cbb6d3df47fb646446019ef5e060e";     // YATRI YDS-013 Aayush Tiwari
// char b_apiKey[] = "key:77a98d9aaf0f8e17a0341564ade6bb5b93765230db433e1f";      // YATRI YDS-011 Sadeep Kharel
// char b_apiKey[] = "key:f7eeb6ca590d615b99e22e3339d8538f644aef2ba9594ef0";     // YATRI YDS-020 Shushil Chitrakar
// char b_apiKey[] = "key:1f1718d2e0232cadf8c657fa6592c2defedc6f6f74d7bea0";     // YATRI YDS-022 Nirdesh Gurung
// char b_apiKey[] = "key:1520857bfc045979727bc07473a1906042e2d8703bb5388e";     // YATRI YDS-026 Shivesh Shrestha
// char b_apiKey[] = "key:a8cca71e4b8fb3841b047e141e136132714a5e3f4540d0ca";     // YATRI YDS-025 Suyash Regmi
// char b_apiKey[] = "key:bcf1094c6e5b57d38067304f0410264889fe727238b1de50";    // YATRI YDS-024 Amin Sutar Karki
// char b_apiKey[] = "key:5818206ddc6c2381ec9d563eec7c7acfc0415a82f71eb123";     // YATRI YDS-010 Sudigya Pant
// char b_apiKey[] = "key:d9791a7c7cc953df4d818ef2041896c35d678cfeed37e4b3";    // YATRI YDS-019 Sushant Bhujel
// char b_apiKey[] = "key:11d2a32f5edfccc0cc8ab72bff0c679379a93ecb32fe591e";     // YATRI YDS-029 Rajesh Shrestha
// char b_apiKey[] = "key:146d22ceb8909466bf768e7eae69556980c9a23250e0d0d1";      // YATRI YDS-012 Shreekar Prajapati
// char b_apiKey[] = "key:33baa88a22e37171e05ba8ef92c7bbfa97b325f840b4fff0";     // YATRI YDS-027 Shreekar Prajapati
// char b_apiKey[] = "key:d7fbbe1071f8cb809ac953c37700b95c56d683afed2674c4";     // YATRI YDS-012 Jeeven Shrestha changed from 030 to 012
// char b_apiKey[] = "key:11d2a32f5edfccc0cc8ab72bff0c679379a93ecb32fe591e";     // YATRI YDS-029 Rajesh Shrestha
// char b_apiKey[] = "key:7f88a89c70e1d13700759fbb887cbc7c0880cfdff2f03ff6";     // YATRI YDS-023 Suryansh Vaidya
// char b_apiKey[] = "key:fa1a655afc9d3ac4d1121ba5a7bcc082b46799838a67e01c";     // YATRI YDS-018 Sudin Gurung
// char b_apiKey[] = "key:7c25a0d4d68d924769b74c561221f986903f42944cd161b7";     // YATRI YDS-029 Basanta Bahadur Tandon
// char b_apiKey[] = "key:33baa88a22e37171e05ba8ef92c7bbfa97b325f840b4fff0";     // YATRI YDS-027 Anand Regmi
// char b_apiKey[] = "key:ec754b7ceaa2ce21d7959f5c31f51f284ae59aa069c1eaa4";     // YATRI YDS-028 Rajesh Karki
// char b_apiKey[] = "key:e48478835b4330c8abf968436c648e04bd5ef61a79c8feaf";     // YATRI YDS-016 Pratik Bhattarai
char b_apiKey[] = "key:b03bd6d53b42149e815a881278de2ae240f69f328fc704c3";     // YATRI YDS-017 Nirmal Mulmi
// char b_apiKey[] = "key:2410e917b7f3dc133cb1530e2f9abbb2c21a38896b5ae1e3";     // YATRI YDS-044 Ram Krishna Khadka

// char b_apiKey[] = "key:b5476df262461839e211c7fcc1eac1843201c5eb1aca45b0";     // Yatri- EMBEDDED TEST BENCH
// char b_apiKey[] = "key:916977745b79a56cda1e16fede699b009d23a6913c8a8f86";     // YATRI YDS-043 Neetika SInnya
// char b_apiKey[] = "key:1cb2739fa958cf5209a9a992509f54fe8fcb248413fe1893";     // Yatri YDS-028 Suprasiddha Tamrakar
// char b_apiKey[] = "key:2fde4334e9bec54359fcbf918fb15d23a2bfe31e047f1b70";     // YATRI YDS-046 Sandeep Thagunna
// char b_apiKey[] = "key:96a823e7b81a13e466519be3630682e25d3f290a04a05c85";     // YATRI YDS-053 Shrijan Acharya 
// char b_apiKey[] = "key:f7f8301b08fc496ca6d6ebf79548a4dbdf799c9a7186dadb";     // YATRI YDS-050 Saugat Dhanchha 
// char b_apiKey[] = "key:f3c59da1e9294c37e5a4647cec3b6ed4f49a046e3ccb102a";     // YATRI YDS-055 Kul Bajracharya 
// char b_apiKey[] = "key:fdab888a96b2875f85f04f5227617593ba7793600156633b";     // YATRI YDS-060 Robin Sharma Dahal
// char b_apiKey[] = "key:cdd82045fa5553942db3c039fdd1aed9d02919efc6d9f90b";     // YATRI YDS-047 Jenish Maharjan  
// char b_apiKey[] = "key:34d1b89ed8333b4ca54b0da6944b88099993184f2cabde27";     // YATRI YDS-049 Addit Thapa
// char b_apiKey[] = "key:bbd63fbc8390c9b2dbf6b32b5ea0e96b6ae83fd64e5654a3";     // Yatri YDS-048 Ajay Shrestha
// char b_apiKey[] = "key:d4aa4234dfba1d22920e450ed4bcd5be176c2959e746ee1a";     // Yatri YDS-052 Ajay Silwal
// char b_apiKey[] = "key:62e05c741413ec7d554d3f8f2c5f3b938a35c2cd61935a85";     // Yatri YDS-071 Nabin Maskey
// char b_apiKey[] = "key:7db86ec60b442cc6e7a7c0acdc784f19d1d7a3e356c9e230";     // Yatri YDS-063 Mabi Gurung
// char b_apiKey[] = "key:1908e0ab75ad372fd806481131c89bfae7a5fa3ce9abd39e";     // Yatri YDS-070 Nirjala Maharjan
// char b_apiKey[] = "key:aa2f0fc550843622ea96d9499acdd6c86c7f60a33a0e23c0";     // Yatri YDS-058 Bibek Manandar
// char b_apiKey[] = "key:231397526862986c5d3845717f8a38104c4d9dd9d17fc008";     // Yatri YDS-061 Prayojan Gyawali
// char b_apiKey[] = "key:e1ec69df363be0426659d3caefad735c103dbf8357e571d6";     // Yatri YDS-064 Prashan Gurung
// char b_apiKey[] = "key:76b87001c233f5976debaf57e9fbb5823951b9b6483944bc";     // Yatri YDS-045 Roshan Shrestha
// char b_apiKey[] = "key:0637d27fec75b5c7bea28deacd0065d871d24857a2565829";     // Yatri YDS-066 Prakash Thapa
// char b_apiKey[] = "key:78ca362141bef4c0f23f1a2e62e345ee413ad155e3ad9c15";     // Yatri YDS-068 Ananda Babu Dhakal

// Bikes readied on 23 Sept
// char b_apiKey[] = "key:442c4905f73970acacc828ddcf15ad2227ddf898c5c91a8e";     // Yatri YDS-077 Jonesh Shrestha

// Bikes readied on 24 Sept
// char b_apiKey[] = "key:6bbb18b20bc82f398c7915604a8c71279827c192f4196620";     // Yatri YDS-086 Aayush Shrestha
// char b_apiKey[] = "key:d6a31eb65d6b4eef42b96d4dfbb3a05b8ffb9200d9a2a7d6";     // Yatri YDS-072 Angel Maharjan
// char b_apiKey[] = "key:d8a1e65c0111492a2e8072e5b497ff9ad7df1fb8184fd9a7";     // Yatri YDS-084 Saroj Rokaya
// char b_apiKey[] = "key:4f913fa960b90da87ca1d7c6b72cf89523d3b3586733abd5";     // Yatri YDS-088 Sujan Basnet
// char b_apiKey[] = "key:5de9623a16d976d5f0a6302bab9b219bb0bc3205c244ab0f";     // Yatri YDS-087 Dipak Barahi
// char b_apiKey[] = "key:11d2a32f5edfccc0cc8ab72bff0c679379a93ecb32fe591e";     // Yatri YDS-031 Basudev Maharjan


// Bikes for Test Riders
// char b_apiKey[] = "key:ca45c11073eb5bed076d555747e19ff8bf82ecaae7dde619";     // Yatri YDS-015 Test Unit 3
// char b_apiKey[] = "key:11d2a32f5edfccc0cc8ab72bff0c679379a93ecb32fe591e";     // Yatri YDS-083 Test Unit 2
// char b_apiKey[] = "key:12f254417d3dc41ff6b0888ac8f42c3323e96cae475ecde8";     // Yatri YDS-085 Test Unit 1


// ----------------------------------------------------------------

#endif

#if 0
/*
 * Yatri Design Studio test server
*/
char bikeVerifyUrl[] = "http://api.embedded.yatridesignstudio.com/api/v1/charge-stations/payments/verify-user";
char ApiKey[] = "key:665ced149c081ce434f7e4aa2ad92d43fdf61d0704a4a6ea";
char crashReportUrl[] = "http://api.embedded.yatridesignstudio.com/api/v1/bikes/crash";
char crashReportApiKey[] = "key:1c599aff61d7395ef020ac3e6f5777b3d9f7f521fb47ddf2";
char bikeLocationUrl[] = "http://api.embedded.yatridesignstudio.com/api/v1/bikes/update-location";
char bikeLocationApiKey[] = "key:af8c5e643481fcb4388e336b5621306737998dde08e3fc91";

char mobileNumber[] = "+9779811093836";  // Enter the Mobile Number you want to send to (EMERGENCY CONTACT ONLY)

//for bike
char b_socUrl[] = "http://api.embedded.yatridesignstudio.com/api/v1/bikes/batteries/soc";
char b_chargeCompleteUrl[] = "http://api.embedded.yatridesignstudio.com/api/v1/bikes/charge-complete";
char b_chargerPluggedUrl[] = "http://api.embedded.yatridesignstudio.com/api/v1/bikes/charge-plugged";
char b_chargerUnpluggedUrl[] = "http://api.embedded.yatridesignstudio.com/api/v1/bikes/charge-unplugged";
char b_telemetryUrl[] = "http://api.embedded.yatridesignstudio.com/api/v1/bikes/telemetries";
char b_theftAlertUrl[] = "http://api.embedded.yatridesignstudio.com/api/v1/bikes/theft-alert";

// API Keys //
char b_apiKey[] = "key:1df6bb339956c8d7eeb00116b9be0d707649c92f9b56ed90";     // Abhay dai
// char b_apiKey[] = "key:31e165db1e70a3d68691f85c7edfe8bbf1f6599769608669";     // Ashim dai
// char b_apiKey[] = "key:1c599aff61d7395ef020ac3e6f5777b3d9f7f521fb47ddf2";     // Saman
 //char b_apiKey[] = "key:d8b9c9670c9717be1f8c74c9e0eb83ec82020fd791c8d0ac";     // user-test4 Sun Glow
// char b_apiKey[] = "key:5199b4723e9c24a35421418eaacbf0d6ee714101456d34ee";     // YDS-041
// char b_apiKey[] = "key:5199b4723e9c24a35421418eaacbf0d6ee714101456d34ee";        // Cloud Burst
#endif
// ----------------------------------------------------------------

char ATcommand[220];
float latitude;
float longitude;

uint8_t rx_buffer[200] = {0};
uint8_t ATisOK = 0;
uint8_t count = 0;
uint8_t retryCount = 0;
uint8_t gps_count = 0;
uint8_t downloadOK = 0;
uint8_t battFlag = 0;
uint8_t gps_flag = 0;
uint8_t gps_count1 = 0;
uint8_t gps_count2 = 0;
uint8_t rxbuff[256];
uint8_t signalStrength;
float battVoltage;
float initial_latitude;
float initial_longitude;
float final_latitude;
float final_longitude;
float distance;


void gpio_init()
{
  gpio_reset_pin(SIM_POWER_PIN);
  gpio_set_direction(SIM_POWER_PIN, GPIO_MODE_OUTPUT);
}

void sim7600_powerup()
{
  uart_write_bytes(uart_port0, "POWERING UP\n", strlen("POWERING UP\n"));
  gpio_set_level(SIM_POWER_PIN, 1);
  vTaskDelay(1000 / portTICK_PERIOD_MS);                          
  gpio_set_level(SIM_POWER_PIN, 0);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, "LEAVING POWERING UP\n", strlen("LEAVING POWERING UP\n"));
}


void sim7600_powerdown()
{
  ATisOK = 0;
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));
  uart_write_bytes(uart_port0, "POWERING DOWN\n", strlen("POWERING DOWN\n"));
  sprintf(ATcommand, "AT+CPOF\r\n");
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  while(!ATisOK)
  {
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    if(strstr((char *)rx_buffer, "OK"))
    {
      ATisOK = 1;
      uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    }
    vTaskDelay(1);
  }
  // uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, "LEAVING POWERING DOWN\n", strlen("LEAVING POWERING DOWN\n"));
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));
}

void sim7600_init()             // SIM7600 INITIALIZATION TEST TASK
{
  ATisOK = 0;
  uart_write_bytes(uart_port0, "VERIFYING SIM MODULE STATUS\n", strlen("VERIFYING SIM MODULE STATUS\n"));
  while(!ATisOK)
  {
    sprintf(ATcommand, "AT\r\n");
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    if(strstr((char *)rx_buffer, "OK"))
    {
      ATisOK = 1;
      count = 0;
      uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    }
    else
    {
      uart_write_bytes(uart_port0, "COUNTING\n", strlen("COUNTING\n"));
      count++;
      if(count == 5)
      {
        uart_write_bytes(uart_port0, "POWERING UP\n", strlen("POWERING UP\n"));
        sim7600_powerup();
      }
    }
    vTaskDelay(1);
  }
  uart_write_bytes(0, "VERIFIED\n", strlen("VERIFIED\n"));
  vTaskDelay(5000 / portTICK_PERIOD_MS);
  ATisOK = 0; 
  sprintf(ATcommand, "AT+CTZU=1\r\n");
  uart_write_bytes(uart_port2, ATcommand, strlen((char *)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
  vTaskDelay(50/ portTICK_PERIOD_MS);
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));
  sprintf(ATcommand, "AT+CTZR=1\r\n");
  uart_write_bytes(uart_port2, ATcommand, strlen((char *)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
  vTaskDelay(50 / portTICK_PERIOD_MS);
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));
  vTaskDelay(50 / portTICK_PERIOD_MS);


  // delete all sms from sim module storage //
  sprintf(ATcommand, "AT+CMGD=1,4\r\n");
  uart_write_bytes(uart_port2, ATcommand, strlen((char *)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 3000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
  vTaskDelay(50/ portTICK_PERIOD_MS);
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));
}

void batt_check()
{
  sprintf(ATcommand, "AT+CBC\r\n");
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);

  char *data1[5] = {0};
  int i1 = 0;
  char delim1[] = " ";
  char *ptr1 = strtok((char *)rx_buffer, delim1);
  while(ptr1 != NULL)
  {
    data1[i1] = ptr1;
    ptr1 = strtok(NULL, delim1);
    i1++;
  }

  char *data2[5] = {0};
  int i2 = 0;
  char delim2[] = "V";
  char *ptr2 = strtok((char *)data1[1], delim2);
  while(ptr2 != NULL)
  {
    data2[i2] = ptr2;
    ptr2 = strtok(NULL, delim2);
    i2++;
  }
  printf("Current Battery Voltage:\n");
  uart_write_bytes(uart_port0, (uint8_t *)data2[0], strlen((char *)data2[0]));
  uart_write_bytes(uart_port0, (uint8_t *)"\n", strlen("\n"));
  battVoltage = atof(data2[0]);

  if(battVoltage < 3.6 && battFlag == 0)
  {
    twai_tx_data[0] = 1;
    twai_tx_data[1] = (int)battVoltage * 10;
    twai_tx(0x18FF0D28, 8);
    battFlag = 1;
  }
  if(battVoltage >= 3.6)
  {
    battFlag = 0;
  }
}

void signal_strength_check()
{
  sprintf(ATcommand, "AT+CSQ\r\n");
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 3000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));

  if(strstr((char *)rx_buffer, "ERROR"))
  {
    printf("ERROR in reading signal strength for now\n");
  }
  else
  {
    char *data1[5] = {0};
    int i1 = 0;
    char delim1[] = " ";
    char *ptr1 = strtok((char *)rx_buffer, delim1);
    while(ptr1 != NULL)
    {
      data1[i1] = ptr1;
      ptr1 = strtok(NULL, delim1);
      i1++;
    }

    char *data2[5] = {0};
    int i2 = 0;
    char delim2[] = ",";
    char *ptr2 = strtok((char *)data1[1], delim2);
    while(ptr2 != NULL)
    {
      data2[i2] = ptr2;
      ptr2 = strtok(NULL, delim2);
      i2++;
    }
    printf("RSSI Signal Strength:\n");
    uart_write_bytes(uart_port0, (uint8_t *)data2[0], strlen((char *)data2[0]));
    uart_write_bytes(uart_port0, (uint8_t *)"\n", strlen("\n"));
    signalStrength = atoi(data2[0]);
  }
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));
}

void sms_task()
{
  sprintf(ATcommand, "AT+CMGF=1\r\n");
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));
  vTaskDelay(100 / portTICK_PERIOD_MS);
  sprintf(ATcommand, "AT+CMGS=\"%s\"\r\n", mobileNumber);
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));
  vTaskDelay(100 / portTICK_PERIOD_MS);
  sprintf(ATcommand, "http://maps.google.com/maps?q=loc:27.7736,85.5536 %c", 0x1a);
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));
  uart_write_bytes(uart_port0, "SMS SENT\n", strlen("SMS SENT\n"));
}

void calc_distance(float lat2, float lat1, float lon2, float lon1)           // (initial latitude, destination latitude, initial longitude, destination longitude)
{
  // Haversine formula
  float a = (sin((lat2 - lat1) * C) / 2) * (sin((lat2 - lat1) * C) / 2) + cos(lat1 * C) * cos(lat2 * C) * ((sin((lon2 - lon1) * C)) / 2) * ((sin((lon2 - lon1) * C)) / 2);
  distance = (2 * 6371 * atan2(sqrt(a), sqrt(1-a)));
  printf("Calculated distance: %f Km\n", distance);
}

void gps_location()
{
  int flag = 0;
  sprintf(ATcommand, "AT+CGPSINFO\r\n");
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
  uart_write_bytes(uart_port0, (uint8_t *)"\n", strlen("\n"));
  memset(ATcommand, 0, sizeof(ATcommand));
  if(strstr((char *)rx_buffer, "SMS") || strstr((char *)rx_buffer, "CCH_PEER_CLOSED"))
  {
    flag = 1;
  }
  if(strlen((char *)rx_buffer) > 80 && flag != 1)    //only parse coordinates if the gps has received the coordinates and use accordingly
  {
    gps_count2 = 19;
    gps_count1++;
    if(gps_count1 == 20)
    {
      twai_tx_data[0] = 1;
      twai_tx(0x18FF1428, 8);
      gps_count1 = 0;
    }

    gps_count = 0;
    char *data1[30] = {0};
    int i1 = 0;
    char delim1[] = " ";
    char *ptr1 = strtok((char *)rx_buffer, delim1);
    while(ptr1 != NULL)
    {
      data1[i1] = ptr1;
      ptr1 = strtok(NULL, delim1);
      i1++;
    }
    uart_write_bytes(uart_port0, (uint8_t *)"\n", strlen("\n"));

    char *data2[30] = {0};
    int i2 = 0;
    char delim2[] = ",";
    char *ptr2 = strtok((char *)data1[1], delim2);
    while(ptr2 != NULL)
    {
      data2[i2] = ptr2;
      ptr2 = strtok(NULL, delim2);
      i2++;
    }

    float lat1;
    lat1 = atof(data2[0])/100;

    float lon1;
    lon1 = atof(data2[2])/100;

    printf("converted lat1: %f,  long1: %f\n", lat1, lon1);
    uart_write_bytes(uart_port0, (uint8_t *)"\n", strlen("\n"));

    latitude = (int)lat1 + (((lat1 - (int)lat1) * 100) / 60);           // final latitude and longitude are saved in Latitude and Longitude global float variables 
    longitude = (int)lon1 + (((lon1 - (int)lon1) * 100) / 60);

    printf("Latitide: %f,  longitude: %f\n", latitude, longitude);    
    uart_write_bytes(uart_port0, (uint8_t *)"\n", strlen("\n"));

    if(gps_flag == 0)
    {
      initial_latitude = latitude;
      initial_longitude = longitude;
      final_latitude = latitude;
      final_longitude = longitude;
      http_post_location_data();
      gps_flag = 1;
    }
    else
    {
      final_latitude = latitude;
      final_longitude = longitude;
    }
    calc_distance(initial_latitude, final_latitude, initial_longitude, final_longitude);
    if(distance >= 0.15)
    {
      http_post_location_data();
      gps_flag = 0;
    }
  }
  else
  {   
    gps_count1 = 19;
    gps_count2++;
    if(gps_count2 == 20)
    {
      twai_tx_data[0] = 0;
      twai_tx(0x18FF1428, 8);
      gps_count2 = 0;
    }

    gps_count++;
    if(gps_count == 30)
    {
      gps_init();
      gps_count = 0;
    }
  }

  memset(rx_buffer, 0, sizeof(rx_buffer));
  vTaskDelay(1);
}

void gps_location_post()
{
  int flag = 0;
  sprintf(ATcommand, "AT+CGPSINFO\r\n");
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
  uart_write_bytes(uart_port0, (uint8_t *)"\n", strlen("\n"));
  memset(ATcommand, 0, sizeof(ATcommand));
  if(strstr((char *)rx_buffer, "SMS") || strstr((char *)rx_buffer, "CCH_PEER_CLOSED"))
  {
    flag = 1;
  }
  if(strlen((char *)rx_buffer) > 80 && flag != 1)    //only parse coordinates if the gps has received the coordinates and use accordingly
  {
    char *data1[30] = {0};
    int i1 = 0;
    char delim1[] = " ";
    char *ptr1 = strtok((char *)rx_buffer, delim1);
    while(ptr1 != NULL)
    {
      data1[i1] = ptr1;
      ptr1 = strtok(NULL, delim1);
      i1++;
    }
    uart_write_bytes(uart_port0, (uint8_t *)"\n", strlen("\n"));

    char *data2[30] = {0};
    int i2 = 0;
    char delim2[] = ",";
    char *ptr2 = strtok((char *)data1[1], delim2);
    while(ptr2 != NULL)
    {
      data2[i2] = ptr2;
      ptr2 = strtok(NULL, delim2);
      i2++;
    }

    float lat1;
    lat1 = atof(data2[0])/100;

    float lon1;
    lon1 = atof(data2[2])/100;

    printf("converted lat1: %f,  long1: %f\n", lat1, lon1);
    uart_write_bytes(uart_port0, (uint8_t *)"\n", strlen("\n"));

    latitude = (int)lat1 + (((lat1 - (int)lat1) * 100) / 60);           // final latitude and longitude are saved in Latitude and Longitude global float variables 
    longitude = (int)lon1 + (((lon1 - (int)lon1) * 100) / 60);

    printf("Latitide: %f,  longitude: %f\n", latitude, longitude);    
    uart_write_bytes(uart_port0, (uint8_t *)"\n", strlen("\n"));
    http_post_location_data();
  }
  memset(rx_buffer, 0, sizeof(rx_buffer));
  vTaskDelay(1);
}

void sms_adxl_value(char xVal[5], char yVal[5], char zVal[5])
{
  sprintf(ATcommand, "AT+CGPSINFO\r\n");
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
  uart_write_bytes(uart_port0, (uint8_t *)"\n", strlen("\n"));
  memset(ATcommand, 0, sizeof(ATcommand));

  if(strlen((char *)rx_buffer) > 80)    //only parse coordinates if the gps has received the coordinates and use accordingly
  {
    char *data1[30] = {0};
    int i1 = 0;
    char delim1[] = " ";
    char *ptr1 = strtok((char *)rx_buffer, delim1);
    while(ptr1 != NULL)
    {
      data1[i1] = ptr1;
      ptr1 = strtok(NULL, delim1);
      i1++;
    }
    uart_write_bytes(uart_port0, (uint8_t *)"\n", strlen("\n"));

    char *data2[30] = {0};
    int i2 = 0;
    char delim2[] = ",";
    char *ptr2 = strtok((char *)data1[1], delim2);
    while(ptr2 != NULL)
    {
      data2[i2] = ptr2;
      ptr2 = strtok(NULL, delim2);
      i2++;
    }
    uart_write_bytes(uart_port0, (uint8_t *)data2[0], strlen((char *)data2[0]));
    uart_write_bytes(uart_port0, (uint8_t *)"\n", strlen("\n"));
    uart_write_bytes(uart_port0, (uint8_t *)data2[2], strlen((char *)data2[2]));
    uart_write_bytes(uart_port0, (uint8_t *)"\n", strlen("\n"));

    float long1, lat1;
    lat1 = atof(data2[0])/100;
    long1 = atof(data2[2])/100;

    printf("converted lat1: %f,  long1: %f\n", lat1, long1);
    uart_write_bytes(uart_port0, (uint8_t *)"\n", strlen("\n"));

    latitude = (int)lat1 + (((lat1 - (int)lat1) * 100) / 60);           // final latitude and longitude are saved in Latitude and Longitude global float variables 
    longitude = (int)long1 + (((long1 - (int)long1) * 100) / 60);

    printf("Latitide: %f,  longitude: %f\n", latitude, longitude);    
    uart_write_bytes(uart_port0, (uint8_t *)"\n", strlen("\n"));

    sprintf(ATcommand, "AT+CMGF=1\r\n");
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));
    vTaskDelay(100 / portTICK_PERIOD_MS);
    sprintf(ATcommand, "AT+CMGS=\"%s\"\r\n", mobileNumber);
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));
    vTaskDelay(100 / portTICK_PERIOD_MS);
    sprintf(ATcommand, "SUSPECTING ACCIDENT OR UNUSUAL ORIENTATION\nX-Val: %sG\nY-Val: %sG\nZ-Val: %sG\nhttp://maps.google.com/maps?q=loc:%f,%f%c", xVal, yVal, zVal, latitude, longitude, 0x1a);
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));
    uart_write_bytes(uart_port0, "SMS SENT\n", strlen("SMS SENT\n"));
    uart_write_bytes(uart_port0, "POSTING CRASH COORDINATES\n", strlen("POSTING CRASH COORDINATES\n"));
    
    http_post_CR_data();
  }

  else
  {
    sprintf(ATcommand, "AT+CMGF=1\r\n");
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));
    vTaskDelay(100 / portTICK_PERIOD_MS);
    sprintf(ATcommand, "AT+CMGS=\"%s\"\r\n", mobileNumber);
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));
    vTaskDelay(100 / portTICK_PERIOD_MS);
    sprintf(ATcommand, "SUSPECTING ACCIDENT OR UNUSUAL ORIENTATION\nX-Val: %sG\nY-Val: %sG\nZ-Val: %sG%c", xVal, yVal, zVal, 0x1a);
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));
    uart_write_bytes(uart_port0, "SMS SENT\n", strlen("SMS SENT\n"));
  }
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));
}

void sms_init()                  // SMS INITIALIZATION TASK
{
  sprintf(ATcommand, "AT+CSMP=17,167,0,0\r\n");
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));
  vTaskDelay(50 / portTICK_PERIOD_MS);
  sprintf(ATcommand, "AT+CMGF=1\r\n");
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));
  uart_write_bytes(uart_port0, "SMS INITIALIZED\n", strlen("SMS INITIALIZED\n"));
}

void gps_init()                 // GPS INITIALIZATION TASK
{
  sprintf(ATcommand, "AT+CGPS=0\r\n");
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 3000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));
  vTaskDelay(50 / portTICK_PERIOD_MS);
  
  sprintf(ATcommand, "AT+CGPS=1,1\r\n");
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));
  vTaskDelay(50 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, "GPS INITIALIZED\n", strlen("GPS INITIALIZED\n"));
}

void get_time()                  //TIME RETRIVAL TASK
{
  uart_write_bytes(uart_port0, "INSIDE GET TIME\n", strlen("INSIDE GET TIME\n"));
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));
  vTaskDelay(50 / portTICK_PERIOD_MS);
  sprintf(ATcommand, "AT+CCLK?\r\n");
  uart_write_bytes(uart_port2, ATcommand, strlen((char *)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));
  char *_data1[5] = {0};
  int _i1 = 0;
  char _delim1[] = ",";
  char *_ptr1 = strtok((char *)rx_buffer, _delim1);
  while(_ptr1 != NULL)
  {
    _data1[_i1] = _ptr1;
    _ptr1 = strtok(NULL, _delim1);
    _i1++;
  }

  uart_write_bytes(uart_port0, (uint8_t *)_data1[0], strlen((char *)_data1[0]));
  uart_write_bytes(uart_port0, (uint8_t *)"\n", strlen("\n"));
  uart_write_bytes(uart_port0, (uint8_t *)_data1[1], strlen((char *)_data1[1]));
  uart_write_bytes(uart_port0, (uint8_t *)"\n", strlen("\n"));

  uart_write_bytes(uart_port0, "PARSING DATE\n", strlen("PARSING DATE\n"));
  char *_data2[5] = {0};
  int _i2 = 0;
  char _delim2[] = "\"";
  char *_ptr2 = strtok((char *)_data1[0], _delim2);
  while(_ptr2 != NULL)
  {
    _data2[_i2] = _ptr2;
    _ptr2 = strtok(NULL, _delim2);
    _i2++;
  }
  uart_write_bytes(uart_port0, (uint8_t *)_data2[1], strlen((char *)_data2[1]));
  uart_write_bytes(uart_port0, (uint8_t *)"\n", strlen("\n"));

  char *_data3[5] = {0};
  int _i3 = 0;
  char _delim3[] = "/";
  char *_ptr3 = strtok((char *)_data2[1], _delim3);
  while(_ptr3 != NULL)
  {
    _data3[_i3] = _ptr3;
    _ptr3 = strtok(NULL, _delim3);
    _i3++;
  }
  printf("Date: \n");
  uart_write_bytes(uart_port0, (uint8_t *)_data3[0], strlen((char *)_data3[0]));
  uart_write_bytes(uart_port0, (uint8_t *)"\n", strlen("\n"));
  uart_write_bytes(uart_port0, (uint8_t *)_data3[1], strlen((char *)_data3[1]));
  uart_write_bytes(uart_port0, (uint8_t *)"\n", strlen("\n"));
  uart_write_bytes(uart_port0, (uint8_t *)_data3[2], strlen((char *)_data3[2]));
  uart_write_bytes(uart_port0, (uint8_t *)"\n", strlen("\n"));


  uart_write_bytes(uart_port0, "PARSING TIME\n", strlen("PARSING TIME\n"));
  char *_data4[5] = {0};
  int _i4 = 0;
  char _delim4[] = "+";
  char *_ptr4 = strtok((char *)_data1[1], _delim4);
  while(_ptr4 != NULL)
  {
    _data4[_i4] = _ptr4;
    _ptr4 = strtok(NULL, _delim4);
    _i4++;
  }
  uart_write_bytes(uart_port0, (uint8_t *)_data4[0], strlen((char *)_data4[0]));
  uart_write_bytes(uart_port0, (uint8_t *)"\n", strlen("\n"));

  uart_write_bytes(uart_port0, "PARSING TIME AND SENDING TO F4 VIA CAN\n", strlen("PARSING TIME AND SENDING TO F4 VIA CAN\n"));
  char *_data5[5] = {0};
  int _i5 = 0;
  char _delim5[] = ":";
  char *_ptr5 = strtok((char *)_data4[0], _delim5);
  while(_ptr5 != NULL)
  {
    _data5[_i5] = _ptr5;
    _ptr5 = strtok(NULL, _delim5);
    _i5++;
  }
  uart_write_bytes(uart_port0, (uint8_t *)_data5[0], strlen((char *)_data5[0]));
  uart_write_bytes(uart_port0, (uint8_t *)"\n", strlen("\n"));
  uart_write_bytes(uart_port0, (uint8_t *)_data5[1], strlen((char *)_data5[1]));
  uart_write_bytes(uart_port0, (uint8_t *)"\n", strlen("\n"));

  twai_tx_data[0] = atoi(_data5[0]);  //hour
  twai_tx_data[1] = atoi(_data5[1]);  //minute
  twai_tx_data[2] = atoi(_data5[2]);  //second
  twai_tx_data[3] = atoi(_data3[0]);  //year
  twai_tx_data[4] = atoi(_data3[1]);  //month
  twai_tx_data[5] = atoi(_data3[2]);  //day

  twai_tx(0x18FF0A28, 8);

  memset(ATcommand, 0, sizeof(ATcommand));
  memset(rx_buffer, 0, sizeof(rx_buffer));
  vTaskDelay(200 / portTICK_PERIOD_MS);
}

void pNumber_check()
{
  sprintf(ATcommand, "AT+CREG?\r\n");
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 3000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));
  vTaskDelay(50 / portTICK_PERIOD_MS);

  sprintf(ATcommand, "AT+COPS?\r\n");
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 3000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));
  vTaskDelay(50 / portTICK_PERIOD_MS);

  sprintf(ATcommand, "AT+CUSD=1\r\n");
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 3000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));
  vTaskDelay(50 / portTICK_PERIOD_MS);

  sprintf(ATcommand, "AT+CUSD=1,\"*903#\"\r\n");
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 5000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));
} 

void balance_check()
{
  sprintf(ATcommand, "AT+CREG?\r\n");
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 3000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));
  vTaskDelay(50 / portTICK_PERIOD_MS);

  sprintf(ATcommand, "AT+COPS?\r\n");
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 3000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));
  vTaskDelay(50 / portTICK_PERIOD_MS);

  sprintf(ATcommand, "AT+CUSD=1\r\n");
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 3000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));
  vTaskDelay(50 / portTICK_PERIOD_MS);

  sprintf(ATcommand, "AT+CUSD=1,\"*901#\"\r\n");
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 5000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));

  if(strstr((char *)rx_buffer, "ERROR") || strstr((char *)rx_buffer, "unavailable"))
  {
    printf("NETWORK PROBLEM, TRY LATER\n");
  }

  else
  {
    char *_data1[5] = {0};
    int _i1 = 0;
    char _delim1[] = ".";
    char *_ptr1 = strtok((char *)rx_buffer, _delim1);
    while(_ptr1 != NULL)
    {
      _data1[_i1] = _ptr1;
      _ptr1 = strtok(NULL, _delim1);
      _i1++;
    }
    char balance[5];
    sprintf(balance, "%s.%s", _data1[1], _data1[2]);
    g_balance = atof(balance);
    printf("Balance: %0.2f", g_balance);
    printf("\n");
  }
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));
}


void ssl_init()                   // SSL COFIGURATION AND LTE ACTIVATION TASK
{
  // activating LTE service and then initializing ssl configurations
  sprintf(ATcommand, "AT+CNMP=2\r\n");
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));
  vTaskDelay(50 / portTICK_PERIOD_MS);

  sprintf(ATcommand, "AT+CSSLCFG=\"sslversion\",0,4\r\n");
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));
  vTaskDelay(50 / portTICK_PERIOD_MS);
  
  sprintf(ATcommand, "AT+CSSLCFG=\"authmode\",0,0\r\n");
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));
  vTaskDelay(50 / portTICK_PERIOD_MS);

  sprintf(ATcommand, "AT+CCHSET=1\r\n");
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));
  vTaskDelay(50 / portTICK_PERIOD_MS);

  sprintf(ATcommand, "AT+CCHSTART\r\n");
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));
  vTaskDelay(50 / portTICK_PERIOD_MS);

  sprintf(ATcommand, "AT+CCHOPEN=0,\"www.yatrimotorcycles.com\",443,2\r\n");
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 3000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));
  vTaskDelay(50 / portTICK_PERIOD_MS);
}

void twai_init()
{
  //Initialize configuration structures using macro initializers
  twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(GPIO_NUM_5, GPIO_NUM_4, TWAI_MODE_NORMAL);
  twai_timing_config_t t_config = TWAI_TIMING_CONFIG_250KBITS();
  twai_filter_config_t f_config = {.acceptance_code = (0x18DF28F0 << 3), .acceptance_mask = ~(0x1FFFFFF0 << 3), .single_filter = false};

  //Install TWAI driver
  if(twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK) 
  {
    printf("Driver installed\n");
  } 
  else 
  {
    printf("Failed to install driver\n");
    return;
  }
  //Start TWAI driver
  if(twai_start() == ESP_OK) 
  {
    printf("Driver started\n");
  } 
  else 
  {
    printf("Failed to start driver\n");
    return;
  }
}

void twai_tx(uint32_t add, uint8_t data_len)
{
  printf("inside twai tx task\n");
  twai_message_t tx_message;
  //configurations for CAN transmission

  tx_message.identifier = add;
  tx_message.extd = 1;
  tx_message.rtr = 0;
  tx_message.data_length_code = data_len;

  for (int i = 0; i < data_len; i++) 
  {
    //twai_tx_data[i] = i;
    tx_message.data[i] = twai_tx_data[i];
  }

  if (twai_transmit(&tx_message, pdMS_TO_TICKS(1000)) == ESP_OK) 
  {
    printf("Message queued for transmission\n");
    twai_clear_transmit_queue();
  } 
  else 
  {
    printf("Failed to queue message for transmission\n");
  }
}

void twai_alert_read()
{
  uint32_t alerts_to_enable = TWAI_ALERT_ERR_PASS | TWAI_ALERT_BUS_OFF | TWAI_ALERT_TX_IDLE | TWAI_ALERT_BUS_ERROR | TWAI_ALERT_RX_QUEUE_FULL;
  if(twai_reconfigure_alerts(alerts_to_enable, NULL) == ESP_OK) 
  {
    printf("Alerts reconfigured\n");
  } 
  else
  {
    printf("Failed to reconfigure alerts");
  }

  uint32_t alerts_triggered;
  twai_read_alerts(&alerts_triggered, pdMS_TO_TICKS(1000));
}

void http_post_CR_data()                    // USER CRASH-REPORT POST
{
  retry:
  uart_write_bytes(uart_port0, "INSIDE CRASH REPORT POST TASK\n", strlen("INSIDE CRASH REPORT POST TASK\n"));
  downloadOK = 0;
  sprintf(ATcommand, "AT+HTTPTERM\r\n");
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));
  
  sprintf(ATcommand, "AT+HTTPINIT\r\n");
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));

  sprintf(ATcommand, "AT+HTTPPARA=\"URL\",\"%s\"\r\n", crashReportUrl);
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));

  sprintf(ATcommand, "AT+HTTPPARA=\"CONTENT\",\"application/json\"\r\n");
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));

  sprintf(ATcommand, "AT+HTTPPARA=\"USERDATA\",\"%s\"\r\n", b_apiKey);
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 3000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));

  sprintf(ATcommand, "{\"x\":%s,\"y\":%s,\"z\":%s,\"location\":{\"coordinates\":[\"%f\",\"%f\"]}}%c", g_xVal, g_yVal, g_zVal, longitude, latitude, 0x0A);
  uint8_t len = strlen(ATcommand);
  memset(ATcommand, 0, sizeof(ATcommand));

  sprintf(ATcommand, "AT+HTTPDATA=%d,10000\r\n", len);
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));

  sprintf(ATcommand, "{\"x\":%s,\"y\":%s,\"z\":%s,\"location\":{\"coordinates\":[\"%f\",\"%f\"]}}%c", g_xVal, g_yVal, g_zVal, longitude, latitude, 0x0A);
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand)); 
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, "INSIDE WHILE\n", strlen("INSIDE WHILE\n"));
  while(!downloadOK)
  {
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 500 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    if(strstr((char *)rx_buffer, "OK"))
    {
      downloadOK = 1;
    }
  }
  uart_write_bytes(uart_port0, "OUTSIDE WHILE\n", strlen("OUTSIDE WHILE\n"));
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));

  sprintf(ATcommand, "AT+HTTPACTION=1\r\n");
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 10000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));

  if(strstr((char *)rx_buffer, "200"))
  {
    uart_write_bytes(uart_port0, "SUCCESSFULLY POSTED\n", strlen("SUCCESSFULLY POSTED\n"));
  }
  else
  {
    if(retryCount < 2)
    {
      retryCount++;
      uart_write_bytes(uart_port0, "POSTING FAILED, RETRY\n", strlen("POSTING FAILED, RETRY\n"));
      goto retry;
    }
    else
    {
      uart_write_bytes(uart_port0, "RETRY FAILED, RESUMING TASK\n", strlen("RETRY FAILED, RESUMING TASK\n"));
      retryCount = 0;
    }
  }

  // sprintf(ATcommand, "AT+HTTPREAD=0,200\r\n");
  // uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  // uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 5000 / portTICK_PERIOD_MS);
  // uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  // uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
  // memset(rx_buffer, 0, sizeof(rx_buffer));
  // memset(ATcommand, 0, sizeof(ATcommand));

  sprintf(ATcommand, "AT+HTTPTERM\r\n");
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));
  
  get_time(); 

  vTaskDelay(1);
}

void http_post_location_data()                   // USER LOCATION POST
{
  //send BUSY state to F4 via CAN
  for(int i = 0; i < 10; i++)
  {
      vTaskDelay(10 / portTICK_PERIOD_MS);
      printf("CAN_STATUS :: Sending BUSY via CAN\n");
      twai_tx_data[0] = 1;
      twai_tx(0x18FF0B28, 8);
  }

  retry:

  // check if network is available or not before posting and if not available, do not post //
  signal_strength_check();
  if(signalStrength > 10)
  {
    uart_write_bytes(uart_port0, "INSIDE LOCATION POST TASK\n", strlen("INSIDE LOCATION POST TASK\n"));
    downloadOK = 0;
    sprintf(ATcommand, "AT+HTTPTERM\r\n");
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));
    
    sprintf(ATcommand, "AT+HTTPINIT\r\n");
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPPARA=\"URL\",\"%s\"\r\n", bikeLocationUrl);
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPPARA=\"CONTENT\",\"application/json\"\r\n");
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPPARA=\"USERDATA\",\"%s\"\r\n", b_apiKey);
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 3000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "{\"location\":{\"coordinates\":[\"%f\",\"%f\"]}}%c", longitude, latitude, 0x0A);
    uint8_t len = strlen(ATcommand);
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPDATA=%d,10000\r\n", len);
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "{\"location\":{\"coordinates\":[\"%f\",\"%f\"]}}%c", longitude, latitude, 0x0A);
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand)); 
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, "INSIDE WHILE\n", strlen("INSIDE WHILE\n"));
    while(!downloadOK)
    {
      uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 500 / portTICK_PERIOD_MS);
      uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
      if(strstr((char *)rx_buffer, "OK"))
      {
        downloadOK = 1;
      }
    }
    vTaskDelay(1);
    uart_write_bytes(uart_port0, "OUTSIDE WHILE\n", strlen("OUTSIDE WHILE\n"));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPACTION=1\r\n");
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 10000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));

    if(strstr((char *)rx_buffer, "200"))
    {
      uart_write_bytes(uart_port0, "SUCCESSFULLY POSTED\n", strlen("SUCCESSFULLY POSTED\n"));
    }
    else
    {
      if(retryCount < 2)
      {
        retryCount++;
        uart_write_bytes(uart_port0, "POSTING FAILED, RETRY\n", strlen("POSTING FAILED, RETRY\n"));
        goto retry;
      }
      else
      {
        uart_write_bytes(uart_port0, "RETRY FAILED, RESUMING TASK\n", strlen("RETRY FAILED, RESUMING TASK\n"));
        retryCount = 0;
      }
    }

    // sprintf(ATcommand, "AT+HTTPREAD=0,200\r\n");
    // uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    // uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 5000 / portTICK_PERIOD_MS);
    // uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    // uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    // memset(rx_buffer, 0, sizeof(rx_buffer));
    // memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPTERM\r\n");
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));

    get_time();
  }
  // else
  // {
  //   sim7600_powerdown();
  //   vTaskDelay(10000 / portTICK_PERIOD_MS);
  //   sim7600_init();
    // ssl_init();
    // gps_init();
    // sms_init();
  // }

  vTaskDelay(2000 / portTICK_PERIOD_MS);
  for(int i = 0; i < 10; i++)
  {
      vTaskDelay(10 / portTICK_PERIOD_MS);
      printf("CAN_STATUS :: Sending IDEAL via CAN\n");
      twai_tx_data[0] = 0;
      twai_tx(0x18FF0B28, 8);
  }

  vTaskDelay(1);
}

void http_post_BV_data(char _bikeId[50])               // BIKE ID VERIFICATION POST
{
  retry:
  uart_write_bytes(uart_port0, "INSIDE BIKE VERIFICATION\n", strlen("INSIDE BIKE VERIFICATION\n"));
  downloadOK = 0;
  sprintf(ATcommand, "AT+HTTPTERM\r\n");
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));
  
  sprintf(ATcommand, "AT+HTTPINIT\r\n");
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));

  sprintf(ATcommand, "AT+HTTPPARA=\"URL\",\"%s\"\r\n", bikeVerifyUrl);
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));

  sprintf(ATcommand, "AT+HTTPPARA=\"CONTENT\",\"application/json\"\r\n");
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));

  sprintf(ATcommand, "AT+HTTPPARA=\"USERDATA\",\"%s\"\r\n", ApiKey);
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 3000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));

  sprintf(ATcommand, "{\"bikeUUID\":\"%s\"}%c", _bikeId, 0x0A);
  uint8_t len = strlen(ATcommand);
  memset(ATcommand, 0, sizeof(ATcommand));

  sprintf(ATcommand, "AT+HTTPDATA=%d,10000\r\n", len);
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));

  sprintf(ATcommand, "{\"bikeUUID\":\"%s\"}%c", _bikeId, 0x0A);
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand)); 
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, "INSIDE WHILE\n", strlen("INSIDE WHILE\n"));
  while(!downloadOK)
  {
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 500 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    if(strstr((char *)rx_buffer, "OK"))
    {
      downloadOK = 1;
    }
  }
  uart_write_bytes(uart_port0, "OUTSIDE WHILE\n", strlen("OUTSIDE WHILE\n"));
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));

  sprintf(ATcommand, "AT+HTTPACTION=1\r\n");
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 10000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));

  if(strstr((char *)rx_buffer, "200"))
  {
    uart_write_bytes(uart_port0, "SUCCESSFULLY POSTED\n", strlen("SUCCESSFULLY POSTED\n"));
  }
  else
  {
    if(retryCount < 2)
    {
      retryCount++;
      uart_write_bytes(uart_port0, "POSTING FAILED, RETRY\n", strlen("POSTING FAILED, RETRY\n"));
      goto retry;
    }
    else
    {
      uart_write_bytes(uart_port0, "RETRY FAILED, RESUMING TASK\n", strlen("RETRY FAILED, RESUMING TASK\n"));
      retryCount = 0;
    }
  }

  // sprintf(ATcommand, "AT+HTTPREAD=0,200\r\n");
  // uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  // uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 5000 / portTICK_PERIOD_MS);
  // uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  // uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
  // memset(rx_buffer, 0, sizeof(rx_buffer));
  // memset(ATcommand, 0, sizeof(ATcommand));

  sprintf(ATcommand, "AT+HTTPTERM\r\n");
  uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
  uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
  uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
  uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
  memset(rx_buffer, 0, sizeof(rx_buffer));
  memset(ATcommand, 0, sizeof(ATcommand));
  retryCount = 0;

  get_time();

  vTaskDelay(1);
}

void http_post_soc_data(char *_soc, char *_chargeStatus, char *_eta)      // SOC, ETA AND CHARGING STATUS POST
{
  retry:

  // check if network is available or not before posting and if not available, do not post //
  signal_strength_check();
  if(signalStrength > 10)
  {
    uart_write_bytes(uart_port0, "INSIDE SOC POST TASK\n", strlen("INSIDE SOC POST TASK\n"));
    char eta1[40];
    char soc1[10];
    char chargeStatus1[10];
    sprintf(eta1, "%d Hours and %d Minutes", atoi(_eta)/60, atoi(_eta)%60);
    strcpy(soc1, _soc);
    strcpy(chargeStatus1, _chargeStatus);
    printf(eta1);
    printf("\n");
    printf(soc1);
    memset(ATcommand, 0, sizeof(ATcommand));
    downloadOK = 0;
    sprintf(ATcommand, "AT+HTTPTERM\r\n");
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));
    
    sprintf(ATcommand, "AT+HTTPINIT\r\n");
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPPARA=\"URL\",\"%s\"\r\n", b_socUrl);
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPPARA=\"CONTENT\",\"application/json\"\r\n");
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPPARA=\"USERDATA\",\"%s\"\r\n", b_apiKey);
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 3000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "{\"soc\":%s,\"isCharging\":%s,\"eta\":\"%s\"}%c", soc1, chargeStatus1, eta1, 0x0A);
    uint8_t len = strlen(ATcommand);
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPDATA=%d,10000\r\n", len);
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "{\"soc\":%s,\"isCharging\":%s,\"eta\":\"%s\"}%c", soc1, chargeStatus1, eta1, 0x0A);
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand)); 
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, "INSIDE WHILE\n", strlen("INSIDE WHILE\n"));
    while(!downloadOK)
    {
      uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 500 / portTICK_PERIOD_MS);
      uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
      if(strstr((char *)rx_buffer, "OK"))
      {
        downloadOK = 1;
      }
    }
    uart_write_bytes(uart_port0, "OUTSIDE WHILE\n", strlen("OUTSIDE WHILE\n"));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPACTION=1\r\n");
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 10000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));

    if(strstr((char *)rx_buffer, "200"))
    {
      uart_write_bytes(uart_port0, "SUCCESSFULLY POSTED\n", strlen("SUCCESSFULLY POSTED\n"));
    }
    else
    {
      if(retryCount < 2)
      {
        retryCount++;
        uart_write_bytes(uart_port0, "POSTING FAILED, RETRY\n", strlen("POSTING FAILED, RETRY\n"));
        goto retry;
      }
      else
      {
        uart_write_bytes(uart_port0, "RETRY FAILED, RESUMING TASK\n", strlen("RETRY FAILED, RESUMING TASK\n"));
        retryCount = 0;
      }
    }

    // memset(rx_buffer, 0, sizeof(rx_buffer));
    // memset(ATcommand, 0, sizeof(ATcommand));
    // sprintf(ATcommand, "AT+HTTPREAD=0,200\r\n");
    // uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    // uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 5000 / portTICK_PERIOD_MS);
    // uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    // uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    // memset(rx_buffer, 0, sizeof(rx_buffer));
    // memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPTERM\r\n");
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));
    retryCount = 0;

    get_time();
  }

  vTaskDelay(1);
}

void http_post_battery_log(char *_soh, char *_lifeCycle, char *_chargeType, char *_startCharge, char *_endCharge, char *_currentODO, char *_ODOvariance)      // SOC, ETA AND CHARGING STATUS POST
{
  retry:

  // check if network is available or not before posting and if not available, do not post //
  signal_strength_check();
  if(signalStrength > 10)
  {
    uart_write_bytes(uart_port0, "INSIDE BATTERY LOG POST TASK\n", strlen("INSIDE BATTERY LOG POST TASK\n"));
    char soh[40];
    char lifeCycle[10];
    char chargeType[30];
    char startCharge[10];
    char endCharge[10];
    char currentODO[10];
    char ODOvariance[10];
    strcpy(soh, _soh);
    strcpy(lifeCycle, _lifeCycle);
    strcpy(chargeType, _chargeType);
    strcpy(startCharge, _startCharge);
    strcpy(endCharge, _endCharge);
    strcpy(currentODO, _currentODO);
    strcpy(ODOvariance, _ODOvariance);

    memset(ATcommand, 0, sizeof(ATcommand));
    downloadOK = 0;
    sprintf(ATcommand, "AT+HTTPTERM\r\n");
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));
    
    sprintf(ATcommand, "AT+HTTPINIT\r\n");
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPPARA=\"URL\",\"%s\"\r\n", b_batteryLogUrl);
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPPARA=\"CONTENT\",\"application/json\"\r\n");
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPPARA=\"USERDATA\",\"%s\"\r\n", b_apiKey);
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 3000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "{\"soh\":%s,\"lifecycle\":%s,\"chargingType\":\"%s\",\"startingCharge\":%s,\"endingCharge\":%s,\"currentOdo\":%s,\"odoVariance\":%s}%c", soh, lifeCycle, chargeType, startCharge, endCharge, currentODO, ODOvariance, 0x0A);
    uint8_t len = strlen(ATcommand);
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPDATA=%d,10000\r\n", len);
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "{\"soh\":%s,\"lifecycle\":%s,\"chargingType\":\"%s\",\"startingCharge\":%s,\"endingCharge\":%s,\"currentOdo\":%s,\"odoVariance\":%s}%c", soh, lifeCycle, chargeType, startCharge, endCharge, currentODO, ODOvariance, 0x0A);
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand)); 
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, "INSIDE WHILE\n", strlen("INSIDE WHILE\n"));
    while(!downloadOK)
    {
      uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 500 / portTICK_PERIOD_MS);
      uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
      if(strstr((char *)rx_buffer, "OK"))
      {
        downloadOK = 1;
      }
    }
    uart_write_bytes(uart_port0, "OUTSIDE WHILE\n", strlen("OUTSIDE WHILE\n"));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPACTION=1\r\n");
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 10000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));

    if(strstr((char *)rx_buffer, "200"))
    {
      uart_write_bytes(uart_port0, "SUCCESSFULLY POSTED\n", strlen("SUCCESSFULLY POSTED\n"));
    }
    else
    {
      if(retryCount < 2)
      {
        retryCount++;
        uart_write_bytes(uart_port0, "POSTING FAILED, RETRY\n", strlen("POSTING FAILED, RETRY\n"));
        goto retry;
      }
      else
      {
        uart_write_bytes(uart_port0, "RETRY FAILED, RESUMING TASK\n", strlen("RETRY FAILED, RESUMING TASK\n"));
        retryCount = 0;
      }
    }

    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));

    // sprintf(ATcommand, "AT+HTTPREAD=0,200\r\n");
    // uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    // uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 5000 / portTICK_PERIOD_MS);
    // uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    // uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    // memset(rx_buffer, 0, sizeof(rx_buffer));
    // memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPTERM\r\n");
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));
    retryCount = 0;

    get_time();
  }

  vTaskDelay(1);
}

void http_post_tele_data(char _disTravelled[5], char _maxSpeed[5], char _avgSpeed[5], char _totalEnergy[10], char _odoSuste[10], char _odoThikka[10], char _odoBabbal[10])     // BIKE TELEMETRY DATA POST
{
  retry:

  // check if network is available or not before posting and if not available, do not post //
  signal_strength_check();
  if(signalStrength > 10)
  {
    uart_write_bytes(uart_port0, "POSTING TELEMETRY DATA\n", strlen("POSTING TELEMETRY DATA\n"));
    char disTravelled1[30];
    char maxSpeed1[10];
    char avgSpeed1[10];
    char totalEnergy1[10];
    char odoSuste1[10];
    char odoThikka1[10];
    char odoBabbal1[10];

    strcpy(disTravelled1, _disTravelled);
    strcpy(maxSpeed1, _maxSpeed);
    strcpy(avgSpeed1, _avgSpeed);
    strcpy(totalEnergy1, _totalEnergy);
    strcpy(odoSuste1, _odoSuste);
    strcpy(odoThikka1, _odoThikka);
    strcpy(odoBabbal1, _odoBabbal);


    memset(ATcommand, 0, sizeof(ATcommand));
    downloadOK = 0;
    sprintf(ATcommand, "AT+HTTPTERM\r\n");
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));
    
    sprintf(ATcommand, "AT+HTTPINIT\r\n");
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPPARA=\"URL\",\"%s\"\r\n", b_telemetryUrl);
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPPARA=\"CONTENT\",\"application/json\"\r\n");
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPPARA=\"USERDATA\",\"%s\"\r\n", b_apiKey);
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 3000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "{\"distanceTravelled\":%s,\"maxSpeed\":%s,\"avgSpeed\":%s,\"totalEUnit\":%s,\"susteOdo\":%s,\"thikkaOdo\":%s,\"babbalOdo\":%s}%c", disTravelled1, maxSpeed1, avgSpeed1, totalEnergy1, odoSuste1, odoThikka1, odoBabbal1, 0x0A);
    uint8_t len = strlen(ATcommand);
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPDATA=%d,10000\r\n", len);
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "{\"distanceTravelled\":%s,\"maxSpeed\":%s,\"avgSpeed\":%s,\"totalEUnit\":%s,\"susteOdo\":%s,\"thikkaOdo\":%s,\"babbalOdo\":%s}%c", disTravelled1, maxSpeed1, avgSpeed1, totalEnergy1, odoSuste1, odoThikka1, odoBabbal1, 0x0A);
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand)); 
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, "INSIDE WHILE\n", strlen("INSIDE WHILE\n"));
    while(!downloadOK)
    {
      uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 500 / portTICK_PERIOD_MS);
      uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
      if(strstr((char *)rx_buffer, "OK"))
      {
        downloadOK = 1;
      }
    }
    uart_write_bytes(uart_port0, "OUTSIDE WHILE\n", strlen("OUTSIDE WHILE\n"));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPACTION=1\r\n");
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 10000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));

    if(strstr((char *)rx_buffer, "200"))
    {
      uart_write_bytes(uart_port0, "SUCCESSFULLY POSTED\n", strlen("SUCCESSFULLY POSTED\n"));
    }
    else
    {
      if(retryCount < 2)
      {
        retryCount++;
        uart_write_bytes(uart_port0, "POSTING FAILED, RETRY\n", strlen("POSTING FAILED, RETRY\n"));
        goto retry;
      }
      else
      {
        uart_write_bytes(uart_port0, "RETRY FAILED, RESUMING TASK\n", strlen("RETRY FAILED, RESUMING TASK\n"));
        retryCount = 0;
      }
    }

    // sprintf(ATcommand, "AT+HTTPREAD=0,200\r\n");
    // uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    // uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 5000 / portTICK_PERIOD_MS);
    // uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    // uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    // memset(rx_buffer, 0, sizeof(rx_buffer));
    // memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPTERM\r\n");
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));
    retryCount = 0;

    get_time();
  }

  vTaskDelay(1);
}

void http_post_charge_completion()                     // CHARGE COMPLETION (100%) NOTIFICATION POST
{
  retry:

  // check if network is available or not before posting and if not available, do not post //
  signal_strength_check();
  if(signalStrength > 10)
  {
    uart_write_bytes(uart_port0, "INSIDE CHARGE COMPLETION NOTIFICATION\n", strlen("INSIDE CHARGE COMPLETION NOTIFICATION\n"));
    sprintf(ATcommand, "AT+HTTPTERM\r\n");
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));
    
    sprintf(ATcommand, "AT+HTTPINIT\r\n");
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPPARA=\"URL\",\"%s\"\r\n", b_chargeCompleteUrl);
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPPARA=\"CONTENT\",\"application/json\"\r\n");
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPPARA=\"USERDATA\",\"%s\"\r\n", b_apiKey);
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 3000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPACTION=1\r\n");
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 10000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));

    if(strstr((char *)rx_buffer, "200"))
    {
      uart_write_bytes(uart_port0, "SUCCESSFULLY POSTED\n", strlen("SUCCESSFULLY POSTED\n"));
    }
    else
    {
      if(retryCount < 2)
      {
        retryCount++;
        uart_write_bytes(uart_port0, "POSTING FAILED, RETRY\n", strlen("POSTING FAILED, RETRY\n"));
        goto retry;
      }
      else
      {
        uart_write_bytes(uart_port0, "RETRY FAILED, RESUMING TASK\n", strlen("RETRY FAILED, RESUMING TASK\n"));
        retryCount = 0;
      }
    }

    // sprintf(ATcommand, "AT+HTTPREAD=0,200\r\n");
    // uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    // uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 5000 / portTICK_PERIOD_MS);
    // uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    // uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    // memset(rx_buffer, 0, sizeof(rx_buffer));
    // memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPTERM\r\n");
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));
    retryCount = 0;

    get_time();
  }

  vTaskDelay(1);
}


void http_post_charge_unplugged(char *_soc)                     // CHARGER UNPLUGGED NOTIFICATION POST
{  
  char soc1[10];
  strcpy(soc1, _soc);
  memset(ATcommand, 0, sizeof(ATcommand));
  downloadOK = 0;
  retry:

  // check if network is available or not before posting and if not available, do not post //
  signal_strength_check();
  if(signalStrength > 10)
  {
    uart_write_bytes(uart_port0, "INSIDE CHARGE UN-PLUGGED NOTIFICATION\n", strlen("INSIDE CHARGE UN-PLUGGED NOTIFICATION\n"));
    sprintf(ATcommand, "AT+HTTPTERM\r\n");
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));
    
    sprintf(ATcommand, "AT+HTTPINIT\r\n");
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPPARA=\"URL\",\"%s\"\r\n", b_chargerUnpluggedUrl);
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPPARA=\"CONTENT\",\"application/json\"\r\n");
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPPARA=\"USERDATA\",\"%s\"\r\n", b_apiKey);
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 3000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "{\"soc\":%s}%c", soc1, 0x0A);
    uint8_t len = strlen(ATcommand);
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPDATA=%d,10000\r\n", len);
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "{\"soc\":%s}%c", soc1, 0x0A);
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand)); 
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, "INSIDE WHILE\n", strlen("INSIDE WHILE\n"));
    while(!downloadOK)
    {
      uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 500 / portTICK_PERIOD_MS);
      uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
      if(strstr((char *)rx_buffer, "OK"))
      {
        downloadOK = 1;
      }
    }
    uart_write_bytes(uart_port0, "OUTSIDE WHILE\n", strlen("OUTSIDE WHILE\n"));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPACTION=1\r\n");
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 10000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));

    if(strstr((char *)rx_buffer, "200"))
    {
      uart_write_bytes(uart_port0, "SUCCESSFULLY POSTED\n", strlen("SUCCESSFULLY POSTED\n"));
    }
    else
    {
      if(retryCount < 2)
      {
        retryCount++;
        uart_write_bytes(uart_port0, "POSTING FAILED, RETRY\n", strlen("POSTING FAILED, RETRY\n"));
        goto retry;
      }
      else
      {
        uart_write_bytes(uart_port0, "RETRY FAILED, RESUMING TASK\n", strlen("RETRY FAILED, RESUMING TASK\n"));
        retryCount = 0;
      }
    }

    // sprintf(ATcommand, "AT+HTTPREAD=0,200\r\n");
    // uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    // uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 5000 / portTICK_PERIOD_MS);
    // uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    // uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    // memset(rx_buffer, 0, sizeof(rx_buffer));
    // memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPTERM\r\n");
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));
    retryCount = 0;

    get_time();
  }

  vTaskDelay(1);
}

void http_post_charge_plugged()             // CHARGER PLUGGED NOTIFICATION POST
{
  retry:

  // check if network is available or not before posting and if not available, do not post //
  signal_strength_check();
  if(signalStrength > 10)
  {
    uart_write_bytes(uart_port0, "INSIDE CHARGE PLUGGED NOTIFICATION\n", strlen("INSIDE CHARGE PLUGGED NOTIFICATION\n"));
    sprintf(ATcommand, "AT+HTTPTERM\r\n");
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));
    
    sprintf(ATcommand, "AT+HTTPINIT\r\n");
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPPARA=\"URL\",\"%s\"\r\n", b_chargerPluggedUrl);
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPPARA=\"CONTENT\",\"application/json\"\r\n");
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPPARA=\"USERDATA\",\"%s\"\r\n", b_apiKey);
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 3000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPACTION=1\r\n");
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 10000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));

    if(strstr((char *)rx_buffer, "200"))
    {
      uart_write_bytes(uart_port0, "SUCCESSFULLY POSTED\n", strlen("SUCCESSFULLY POSTED\n"));
    }
    else
    {
      if(retryCount < 2)
      {
        retryCount++;
        uart_write_bytes(uart_port0, "POSTING FAILED, RETRY\n", strlen("POSTING FAILED, RETRY\n"));
        goto retry;
      }
      else
      {
        uart_write_bytes(uart_port0, "RETRY FAILED, RESUMING TASK\n", strlen("RETRY FAILED, RESUMING TASK\n"));
        retryCount = 0;
      }
    }

    // sprintf(ATcommand, "AT+HTTPREAD=0,200\r\n");
    // uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    // uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 5000 / portTICK_PERIOD_MS);
    // uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    // uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    // memset(rx_buffer, 0, sizeof(rx_buffer));
    // memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPTERM\r\n");
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));
    retryCount = 0;

    get_time();
  }

  vTaskDelay(1);
}

void theft_alert()
{
  retry:

  // check if network is available or not before posting and if not available, do not post //
  signal_strength_check();
  if(signalStrength > 10)
  {
    uart_write_bytes(uart_port0, "INSIDE THEFT ALERT NOTIFICATION\n", strlen("INSIDE THEFT ALERT NOTIFICATION\n"));
    sprintf(ATcommand, "AT+HTTPTERM\r\n");
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));
    
    sprintf(ATcommand, "AT+HTTPINIT\r\n");
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPPARA=\"URL\",\"%s\"\r\n", b_theftAlertUrl);
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPPARA=\"CONTENT\",\"application/json\"\r\n");
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPPARA=\"USERDATA\",\"%s\"\r\n", b_apiKey);
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 3000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPACTION=1\r\n");
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 10000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));

    if(strstr((char *)rx_buffer, "200"))
    {
      uart_write_bytes(uart_port0, "SUCCESSFULLY POSTED\n", strlen("SUCCESSFULLY POSTED\n"));
    }
    else
    {
      if(retryCount < 2)
      {
        retryCount++;
        uart_write_bytes(uart_port0, "POSTING FAILED, RETRY\n", strlen("POSTING FAILED, RETRY\n"));
        goto retry;
      }
      else
      {
        uart_write_bytes(uart_port0, "RETRY FAILED, RESUMING TASK\n", strlen("RETRY FAILED, RESUMING TASK\n"));
        retryCount = 0;
      }
    }

    // sprintf(ATcommand, "AT+HTTPREAD=0,200\r\n");
    // uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    // uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 5000 / portTICK_PERIOD_MS);
    // uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    // uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    // memset(rx_buffer, 0, sizeof(rx_buffer));
    // memset(ATcommand, 0, sizeof(ATcommand));

    sprintf(ATcommand, "AT+HTTPTERM\r\n");
    uart_write_bytes(uart_port2, ATcommand, strlen((char*)ATcommand));
    uart_read_bytes(uart_port2, rx_buffer, BUF_SIZE, 1000 / portTICK_PERIOD_MS);
    uart_write_bytes(uart_port0, ATcommand, strlen((char *)ATcommand));
    uart_write_bytes(uart_port0, rx_buffer, strlen((char *)rx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));
    memset(ATcommand, 0, sizeof(ATcommand));
    retryCount = 0;

    get_time();
  }
  
  vTaskDelay(1);
}

void uart_init()                    // UART INITIALIZATION TASK
{
  uart_config_t uart_config0 = {
  .baud_rate = 115200,
  .data_bits = UART_DATA_8_BITS,
  .parity    = UART_PARITY_DISABLE,
  .stop_bits = UART_STOP_BITS_1,
  .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
  };
 
  uart_config_t uart_config2 = {
  .baud_rate = 115200,
  .data_bits = UART_DATA_8_BITS,
  .parity    = UART_PARITY_DISABLE,
  .stop_bits = UART_STOP_BITS_1,
  .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
  };
  uart_param_config(uart_port0, &uart_config0);
  uart_param_config(uart_port2, &uart_config2);

  uart_set_pin(uart_port0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
  uart_set_pin(uart_port2, UART_2_TX, UART_2_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

  uart_driver_install(uart_port0, BUF_SIZE * 2, 0, 0, NULL, 0);
  uart_driver_install(uart_port2, BUF_SIZE * 2, 0, 0, NULL, 0);
}