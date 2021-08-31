#include "main.h"
#include "stm32f4xx_hal.h"

/* Defines ------------------------------------------------------------------*/
#define FINGERPRINT_OK 0x00
#define FINGERPRINT_PACKETRECIEVEERR 0x01
#define FINGERPRINT_NOFINGER 0x02
#define FINGERPRINT_IMAGEFAIL 0x03
#define FINGERPRINT_IMAGEMESS 0x06
#define FINGERPRINT_FEATUREFAIL 0x07
#define FINGERPRINT_NOMATCH 0x08
#define FINGERPRINT_NOTFOUND 0x09
#define FINGERPRINT_ENROLLMISMATCH 0x0A
#define FINGERPRINT_BADLOCATION 0x0B
#define FINGERPRINT_DBRANGEFAIL 0x0C
#define FINGERPRINT_UPLOADFEATUREFAIL 0x0D
#define FINGERPRINT_PACKETRESPONSEFAIL 0x0E
#define FINGERPRINT_UPLOADFAIL 0x0F
#define FINGERPRINT_DELETEFAIL 0x10
#define FINGERPRINT_DBCLEARFAIL 0x11
#define FINGERPRINT_PASSFAIL 0x13
#define FINGERPRINT_INVALIDIMAGE 0x15
#define FINGERPRINT_FLASHERR 0x18
#define FINGERPRINT_INVALIDREG 0x1A
#define FINGERPRINT_ADDRCODE 0x20
#define FINGERPRINT_PASSVERIFY 0x21

#define FINGERPRINT_STARTCODE 0xEF01

#define FINGERPRINT_COMMANDPACKET 0x1
#define FINGERPRINT_DATAPACKET 0x2
#define FINGERPRINT_ACKPACKET 0x7
#define FINGERPRINT_ENDDATAPACKET 0x8

#define FINGERPRINT_TIMEOUT 0xFF
#define FINGERPRINT_BADPACKET 0xFE

#define FINGERPRINT_GETIMAGE 0x01
#define FINGERPRINT_IMAGE2TZ 0x02
#define FINGERPRINT_REGMODEL 0x05
#define FINGERPRINT_STORE 0x06
#define FINGERPRINT_LOAD 0x07
#define FINGERPRINT_UPLOAD 0x08
#define FINGERPRINT_DELETE 0x0C
#define FINGERPRINT_EMPTY 0x0D
#define FINGERPRINT_VERIFYPASSWORD 0x13
#define FINGERPRINT_HISPEEDSEARCH 0x1B
#define FINGERPRINT_TEMPLATECOUNT 0x1D

#define DEFAULTTIMEOUT 5000  // mili giay

/* Function prototypes -----------------------------------------------*/
void UART1_FINGER_Init(void); //cau hinh uart

uint8_t verifyPassword(void);		//xac minh password
int8_t getImage(void);					//lay hinh anh van tay
int8_t image2Tz(uint8_t slot);	//luu vao buffer
int8_t createModel(void);				//tong hop tao mau

int8_t emptyDatabase(void);				//xoa tat ca van tay
int8_t storeModel(uint16_t id);		//luu mau van tay
int8_t deleteModel(uint16_t id);	//xoa 1 mau van tay
int8_t fingerFastSearch(void);
void writePacket(uint32_t addr, uint8_t packettype, uint16_t len, uint8_t *packet);
uint8_t getReply(uint8_t packet[]);

uint8_t fingerEnroll(uint8_t id);		//dang ky van tay moi voi id
int8_t fingerIDSearch(void);				//tim id van tay

