#include "R305.h"
#include "stdio.h"
#include "STM_MY_LCD16X2.h"
UART_HandleTypeDef uartFinger;

uint16_t fingerID, confidence;
uint8_t recvPacket[20];
uint32_t thePassword = 0;
uint32_t theAddress = 0xFFFFFFFF;

/* USART init function */
void UART3_FINGER_Init(void)
{
  uartFinger.Instance = USART3;
  uartFinger.Init.BaudRate = 57600;
  uartFinger.Init.WordLength = UART_WORDLENGTH_8B;
  uartFinger.Init.StopBits = UART_STOPBITS_1;
  uartFinger.Init.Parity = UART_PARITY_NONE;
  uartFinger.Init.Mode = UART_MODE_TX_RX;
  uartFinger.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  uartFinger.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&uartFinger) != HAL_OK)
  {
    Error_Handler();
  }

}

/* FINGERPRINT function */
void writePacket(uint32_t addr, uint8_t packettype, uint16_t len, uint8_t *packet)
{
	uint16_t sum;
	uint8_t i;

	uint8_t startcodeH = FINGERPRINT_STARTCODE >> 8;
	uint8_t startcodeL = FINGERPRINT_STARTCODE&0xff;
	uint8_t addr1 = addr >> 24;
	uint8_t addr2 = addr >> 16;
	uint8_t addr3 = addr >> 8;
	uint8_t addr4 = addr&0xff;
	uint8_t lenH = len>>8;
	uint8_t lenL = len&0xff;
	HAL_UART_Transmit(&uartFinger, &startcodeH, 1, DEFAULTTIMEOUT);
  HAL_UART_Transmit(&uartFinger, &startcodeL, 1, DEFAULTTIMEOUT);
  HAL_UART_Transmit(&uartFinger, &addr1, 1, DEFAULTTIMEOUT);
	HAL_UART_Transmit(&uartFinger, &addr2, 1, DEFAULTTIMEOUT);
  HAL_UART_Transmit(&uartFinger, &addr3, 1, DEFAULTTIMEOUT);
  HAL_UART_Transmit(&uartFinger, &addr4, 1, DEFAULTTIMEOUT);
  HAL_UART_Transmit(&uartFinger, &packettype, 1, DEFAULTTIMEOUT);
  HAL_UART_Transmit(&uartFinger, &lenH, 1, DEFAULTTIMEOUT);
  HAL_UART_Transmit(&uartFinger, &lenL, 1, DEFAULTTIMEOUT);
 
  sum = (len>>8) + (len&0xff) + packettype;
	uint8_t packet_int8;
  for (i=0; i< len-2; i++) {
		packet_int8 = packet[i];
    HAL_UART_Transmit(&uartFinger, &packet_int8, 1, DEFAULTTIMEOUT);
    sum += packet[i];
  }
	
	uint8_t sumH = sum>>8;
	uint8_t sumL = sum&0xff;
  HAL_UART_Transmit(&uartFinger, &sumH, 1, DEFAULTTIMEOUT);
  HAL_UART_Transmit(&uartFinger, &sumL, 1, DEFAULTTIMEOUT);
}

uint8_t getReply(uint8_t packet[])
{
  uint8_t reply[20], idx, i, packettype;
	uint16_t len;
	idx = 0;

  while (1)
  {
		//cho RX san sang
    while (uartFinger.RxState != HAL_UART_STATE_READY) {}
    //nhan packet tu cam bien van tay
    HAL_UART_Receive(&uartFinger, &reply[idx], 1, DEFAULTTIMEOUT);
    if ((idx == 0) && (reply[0] != (FINGERPRINT_STARTCODE >> 8)))
      continue;
    idx++;
    
    // kiem tra packet!
    if (idx >= 9) {
      if ((reply[0] != (FINGERPRINT_STARTCODE >> 8)) ||
          (reply[1] != (FINGERPRINT_STARTCODE & 0xFF)))
          return FINGERPRINT_BADPACKET;
			
      packettype = reply[6];
      len = reply[7];
			
      len <<= 8;
      len |= reply[8];
      len -= 2;
			
      if (idx <= (len+10)) continue;
      packet[0] = packettype;      
      for (i=0; i<len; i++) {
        packet[1+i] = reply[9+i];
      }
      return len;
    }
  }
}

uint8_t verifyPassword(void)
{
	uint8_t len;
	
	uint8_t packet[] = {FINGERPRINT_VERIFYPASSWORD, 
                      (thePassword >> 24), (thePassword >> 16),
                      (thePassword >> 8), thePassword};
	
  writePacket(theAddress, FINGERPRINT_COMMANDPACKET, 7, packet);
  len = getReply(recvPacket);
  
  if ((len == 1) && (recvPacket[0] == FINGERPRINT_ACKPACKET) && (recvPacket[1] == FINGERPRINT_OK))
    return 1;

  return 0;
}

int8_t getImage(void)
{
	uint8_t len;
  uint8_t packet[] = {FINGERPRINT_GETIMAGE};
  writePacket(theAddress, FINGERPRINT_COMMANDPACKET, 3, packet);
  len = getReply(recvPacket);
  
  if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
   return -1;
  return recvPacket[1];
}

int8_t image2Tz(uint8_t slot)
{
	uint8_t len;
	uint8_t packet[] = {FINGERPRINT_IMAGE2TZ, slot};
	
  writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet)+2, packet);
  len = getReply(recvPacket);
  
  if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
   return -1;
  return recvPacket[1];
}

int8_t createModel(void)
{
	uint8_t len;
  uint8_t packet[] = {FINGERPRINT_REGMODEL};
	
  writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet)+2, packet);
  len = getReply(recvPacket);
  
  if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
   return -1;
  return recvPacket[1];
}

int8_t storeModel(uint16_t id)
{
	uint8_t len;
	uint8_t packet[] = {FINGERPRINT_STORE, 0x02, 0, 0};
	packet[2] = id >> 8;
	packet[3] = id & 0xFF;
	
  writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet)+2, packet);
  len = getReply(recvPacket);
  
  if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
   return -1;
  return recvPacket[1];
}

int8_t deleteModel(uint16_t id)
{
		uint8_t len;
    uint8_t packet[] = {FINGERPRINT_DELETE, id >> 8, id & 0xFF, 0x00, 0x01};
		packet[1] = id >> 8;
		packet[2] = id & 0xFF;
		
    writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet)+2, packet);
    len = getReply(recvPacket);
        
    if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
        return -1;
    return recvPacket[1];
}

int8_t emptyDatabase(void)
{
	uint8_t len;
  uint8_t packet[] = {FINGERPRINT_EMPTY};
	
  writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet)+2, packet);
  len = getReply(recvPacket);
  
  if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
   return -1;
  return recvPacket[1];
}

int8_t fingerFastSearch(void)
{
	uint8_t len;
  uint8_t packet[] = {FINGERPRINT_HISPEEDSEARCH, 0x01, 0x00, 0x00, 0x00, 0xA3};
	
	fingerID = 0xFFFF;
  confidence = 0xFFFF;
	
  writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet)+2, packet);
  len = getReply(recvPacket);
  
  if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
   return -1;

  fingerID = recvPacket[2];
  fingerID <<= 8;
  fingerID |= recvPacket[3];
  
  confidence = recvPacket[4];
  confidence <<= 8;
  confidence |= recvPacket[5];
  
  return recvPacket[1];
}


uint8_t fingerEnroll(uint8_t id)
{
	int8_t p = -1;
	
//	LCD16X2_Fingerprint("Put your finger.");
	LCD1602_setCursor(2,1);
	LCD1602_print("Moi dat tay     ");
	
	while (p != FINGERPRINT_OK)
	{
    p = getImage();
	}
	
	p = image2Tz(1);
	if(p != FINGERPRINT_OK)
	{ LCD1602_clear();
//		LCD16X2_Fingerprint("Error.");
		LCD1602_setCursor(2,1);
		LCD1602_print("loi tao ky tu 1 ");
		return 0;
	}
//	LCD16X2_Fingerprint("Captured.");
	LCD1602_setCursor(2,1);
	LCD1602_print("dang capture   ");
	HAL_Delay(1000);
	p = -1;
	while (p != FINGERPRINT_NOFINGER)
	{
    p = getImage();
  }
	
//	LCD16X2_Fingerprint("Confirm finger.");
	LCD1602_setCursor(2,1);
	LCD1602_print("da luu charbuf1 ");

	p = -1;
	while (p != FINGERPRINT_OK) {
    p = getImage();
	}
	
	p = image2Tz(2);
	if(p != FINGERPRINT_OK)
	{
//		LCD16X2_Fingerprint("Error.");
		LCD1602_setCursor(2,1);
		LCD1602_print("loi tao ky tu 2  ");
		return 0;
	}
	
	p = createModel();
	if(p != FINGERPRINT_OK)
	{
//		LCD16X2_Fingerprint("Error.");
		LCD1602_setCursor(2,1);
		LCD1602_print("loi khi tao mau  ");
		return 0;
	}
	
	p = storeModel(id);
	if (p == FINGERPRINT_OK)
	{
//		LCD16X2_Fingerprint("Success.");
		LCD1602_setCursor(2,1);
		LCD1602_print("success          ");
		return 1;
	}
	else
	{
//		LCD16X2_Fingerprint("Faile.");
		LCD1602_setCursor(2,1);
		LCD1602_print("that bai khi tao");
		return 0;
	}
}

int8_t fingerIDSearch(void)
{
	int8_t p = -1;
	
//	LCD16X2_Fingerprint("Put your finger.");
	LCD1602_setCursor(2,1);
	LCD1602_print("put you finger  ");
	
	while(p != FINGERPRINT_OK)
	{
		p = getImage();
	}

  p = image2Tz(1);
  if (p != FINGERPRINT_OK)
	{
//		LCD16X2_Fingerprint("Error.");
		LCD1602_setCursor(2,1);
		LCD1602_print("loi tao kytu ");
		return -1;
	}	
	HAL_Delay(1000);
	
  p = fingerFastSearch();
  if (p != FINGERPRINT_OK)
	{
//		LCD16X2_Fingerprint("Not Found.");
		LCD1602_setCursor(2,1);
		LCD1602_print("notfound        ");
		return -1;
	}
LCD1602_setCursor(2,1);
LCD1602_print("found           ");
//	LCD16X2_Fingerprint("Found: ");
//	LCD16X2_PutChar(fingerID/10%10 + 0x30);
//	LCD16X2_PutChar(fingerID%10 + 0x30);

  return fingerID; 
}

