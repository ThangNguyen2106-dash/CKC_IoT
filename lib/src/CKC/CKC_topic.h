#ifndef INC_CKC_TOPIC
#define INC_CKC_TOPIC

/*
Topic format:
    ckc/chip/<token>/<prefix topic>
 example SUB:     ckc/chip/PLG2026/arduino_pin/   mess  <PAYLOAD>
 example PUB:     ckc/chip/PLG2026/infor            mess  <DATA>
*/
#define CKC_BASE_TOPIC "ckc/chip/"

/* Subscribe topic */
#define CKC_SUB_PREFIX_DOWN_TOPIC "/down"
#define CKC_SUB_PREFIX_ARDUINO_TOPIC "/arduino_pin/"
#define CKC_SUB_PREFIX_VIRTUAL_TOPIC "/virtual_pin/"
#define CKC_SUB_PREFIX_ASK_WIFI_TOPIC "/wifi/ask"
#define CKC_SUB_PREFIX_CHANGE_WIFI_TOPIC "/wifi/change"

/* Publish topic */
#define CKC_PUB_PREFIX_INFO_TOPIC "/info"
#define CKC_PUB_PREFIX_MODBUS_DATA_TOPIC "/data/"
#define CKC_PUB_PREFIX_LIST_WIFI_TOPIC "/wifi/list"

#endif // INC_CKC_TOPIC