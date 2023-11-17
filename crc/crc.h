#ifndef __CRC_H__
#define __CRC_H__

#include "types.h"

//定義所用的CRC校驗值
typedef struct{
	u8 POLY;//多項式
	u8 INIT;//初始值
	u8 XOR;//异或值
	BOOL REFIN;//輸入反轉判斷
	BOOL REFOUT;//輸出反轉判斷
}CRC_8;

typedef struct{
	u16 POLY;//多項式
	u16 INIT;//初始值
	u16 XOR;//异或值
	BOOL REFIN;//輸入反轉判斷
	BOOL REFOUT;//輸出反轉判斷
}CRC_16;

typedef struct{
	u32 POLY;//多項式
	u32 INIT;//初始值
	u32 XOR;//异或值
	BOOL REFIN;//輸入反轉判斷
	BOOL REFOUT;//輸出反轉判斷
}CRC_32;

//聲明CRC變量，使用const關鍵字防止修改
const CRC_8 CRC8 = {0x07, 0x00, 0x00, FALSE, FALSE};
const CRC_8 CRC8ITU = {0x07, 0x00, 0x55, FALSE, FALSE};
const CRC_8 CRC8ROHC = {0x07, 0xFF, 0x00, TRUE, TRUE};
const CRC_8 CRC8MAXIM = {0x31, 0x00, 0x00, TRUE, TRUE};
const CRC_16 CRC16IBM = {0x8005, 0x0000, 0x0000, TRUE, TRUE};
const CRC_16 CRC16MAXIM = {0x8005, 0x0000, 0xFFFF, TRUE, TRUE};
const CRC_16 CRC16USB = {0x8005, 0xFFFF, 0xFFFF, TRUE, TRUE};
const CRC_16 CRC16MODBUS = {0x8005, 0xFFFF, 0x0000, TRUE, TRUE};
const CRC_16 CRC16CCITT = {0x1021, 0x0000, 0x0000, TRUE, TRUE};
const CRC_16 CRC16XMODEM = {0x1021, 0x0000, 0x0000, FALSE, FALSE};
const CRC_32 CRC32 = {0x04C11DB7, 0xFFFFFFFF, 0xFFFFFFFF, TRUE, TRUE};
const CRC_32 CRC32MPEG2 = {0x04C11DB7, 0xFFFFFFFF, 0x00000000, FALSE, FALSE};

u8 CheckCRC8(const u8 * addr, int num, CRC_8 type);
u16 CheckCRC16(const u8 * addr, int num, CRC_16 type);
u32 CheckCRC32(const u8 * addr, int num, CRC_32 type);

u16 CRC16_MAXIM(const u8 * addr, int num);

#endif

