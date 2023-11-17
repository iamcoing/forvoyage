#include "types.h"
#include "crc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * 功能名称：byteReser
 * 功能描述：将给定的单位字节按位反转
 * byte：用于反转的字节
 * tmp ：返回反转后的字节
 */
static u8 byteReser(u8 byte)
{
	u8 tmp = 0;
	for(int i = 0; i < 4; i++)
	{
		tmp |= (byte & 0x01 << i) >> i << (7 - i); 
		tmp |= (byte & 0x80 >> i) << i >> (7 - i);
	}
	
	return tmp;
}

static void dataReser(u8 * data, int num)
{
	//进行字符串反转
	u8 * head = data;
	u8 * tail = data + num - 1;
	u8 tmp = 0;
	while(tail >= head){
		tmp	= *tail;
		*tail = *head;
		*head = tmp;
		head++;
		tail--;
	}
	
	//将数据按位反转
	for(int i = 0; i < num; i++){
		*(data + i) = byteReser(*(data + i));
	}
	return;
}

/*
 * 功能名称：CheckCRC8
 * 功能描述：给定字符串，计算CRC8
 * addr: 校验起始位的地址
 * num:  校验的长度(字节)
 * type: 使用的校验类型
 */
u8 CheckCRC8(const u8 * addr, int num, CRC_8 type)
{
	u8 crc = type.INIT;
	u8 data;
	u8 * tmp = (u8 *)malloc((num + 1) * sizeof(u8));
	if(tmp == NULL)
	{
		perror("tmp");
		return 0;
	}
	//拷贝数据到tmp
	strcpy(tmp, addr);
	if(type.REFIN){
	//反转函数
		int i = num;
		while(i > 0){
			i--;
			*(tmp + i) = byteReser(*(tmp + i));
		}
	}
	for(int i = 0; i < num; i++)
	{
		data = *(tmp + i);
		//将crc与data的高8位相异或
		crc  = crc ^ (data << (sizeof(crc) - 1) * 8);
		for(int i = 0 ; i < 8; i++)
		{
			//判断当前CRC最高位是否为1
			if( crc & (0x01 << sizeof(crc) * 8 - 1)){
				crc = (crc << 1) ^ type.POLY;
			}
			else{
				crc <<= 1;
			}
		}
	}
	if(type.REFOUT){
		//反转函数，crc
		dataReser((u8 *)&crc, 1);
	}
	crc ^= type.XOR;
	free(tmp);

	return crc;

}

u16 CheckCRC16(const u8 * addr, int num, CRC_16 type)
{
	u8 data;
	u16 crc = type.INIT;
	u8 * tmp = (u8 *)malloc((num + 1) * sizeof(u8));
	if(tmp == NULL)
	{
		perror("tmp");
		return 0;
	}
	//拷贝数据到tmp
	strcpy(tmp, addr);
	if(type.REFIN){
	//反转函数
		int i = num;
		while(i > 0){
			i--;
			*(tmp + i) = byteReser(*(tmp + i));
		}
	}
	for(int i = 0; i < num; i++)
	{
		data = *(tmp + i);
		//将crc与data的高8位相异或
		crc  = crc ^ (data << (sizeof(crc) - 1) * 8);
		for(int i = 0 ; i < 8; i++)
		{
			//判断当前CRC最高位是否为1
			if( crc & (0x01 << sizeof(crc) * 8 - 1)){
				crc = (crc << 1) ^ type.POLY;
			}
			else{
				crc <<= 1;
			}
		}
	}
	if(type.REFOUT){
		//反转函数，crc
		dataReser((u8 *)&crc, 2);
	}
	crc ^= type.XOR;
	free(tmp);

	return crc;
}

/*
 * 功能名称：CRC16_MAXIM
 * 功能描述：给定字符串, 计算CRC16MAXIM类型的CRC校验值
 * addr: 校验起始位的地址
 * num:  校验的长度(字节)
 * 改进点：针对常用的CRC计算，避免判断循环和反转的时间
 * */
u16 CRC16_MAXIM(const u8 * addr, int num)
{
	u8 data;
	u16 crc = 0x0000;
	int i;
	//移位等操作需要与之前的方向相反了
	for(; num > 0; num--){
		//同低8位异或
		crc = crc ^ *addr++;
		for(i = 0; i < 8; i++){
			if(crc & 0x1)
			 //这里需要把多项式全部反转，用来获得全反转的CRC
				crc = (crc >> 1) ^ 0xA001;
			else
				crc >>= 1;
		}
	}
	
	return (crc ^ 0xffff);
}

u32 CheckCRC32(const u8 * addr, int num, CRC_32 type)
{
	u8 data;
	u32 crc = type.INIT;
	u8 * tmp = (u8 *)malloc((num + 1) * sizeof(u8));
	if(tmp == NULL)
	{
		perror("tmp");
		return 0;
	}
	//拷贝数据到tmp
	strcpy(tmp, addr);
	if(type.REFIN){
	//反转函数
		int i = num;
		while(i > 0){
			i--;
			*(tmp + i) = byteReser(*(tmp + i));
		}
	}
	for(int i = 0; i < num; i++)
	{
		data = *(tmp + i);
		//将crc与data的高8位相异或
		crc  = crc ^ (data << (sizeof(crc) - 1) * 8);
		for(int i = 0 ; i < 8; i++)
		{
			//判断当前CRC最高位是否为1
			if( crc & (0x01 << sizeof(crc) * 8 - 1)){
				crc = (crc << 1) ^ type.POLY;
			}
			else{
				crc <<= 1;
			}
		}
	}
	if(type.REFOUT){
		printf("in : %#x\n", crc);
		//反转函数，crc
		dataReser((u8 *)&crc, 4);
		printf("out: %#x\n", crc);
	}
	crc ^= type.XOR;
	free(tmp);

	return crc;
}

int main(int argc, const char * argv[])
{
	u8 str[12] = "hello world";
	printf("%#x\n", CheckCRC16(str, sizeof(str) - 1, CRC16USB));
	return 0;
}

