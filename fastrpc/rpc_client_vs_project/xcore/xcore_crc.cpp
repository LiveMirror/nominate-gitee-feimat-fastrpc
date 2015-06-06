// 2008-06-10
// xcore_crc.cpp
// 
// CRC检验类


#include "xcore_crc.h"

namespace xcore {

#define CRCCCITT 0x1021
#define CCITT_REV 0x8408
#define CRC16 0x8005
#define CRC16_REV 0xA001
#define CRC32 0X04C11DB7
#define CRC32_POLYNOMIAL 0xEDB88320

uint32 crc_32(const void* src, uint32 len, uint32 checksum)
{
	// 表的生成算法
	/*uint32 table[256];
	for (uint32 i = 0; i < 256; ++i)
		for (uint32 reg = i << 24, j = 0; j < 8; ++j)
			table[i] = reg = (reg & 0x80000000) ? ( reg << 1) ^ 0X04C11DB7 : (reg << 1);*/

	static uint32 crctable[256] = {     // X32+X26+X23+...+X2+X+1 余式表 (0X04C11DB7)
		0X00000000, 0XEDB88320, 0X36C98560, 0XDB710640, 0X6D930AC0, 0X802B89E0, 0X5B5A8FA0, 0XB6E20C80,
		0XDB261580, 0X369E96A0, 0XEDEF90E0, 0X005713C0, 0XB6B51F40, 0X5B0D9C60, 0X807C9A20, 0X6DC41900,
		0X5BF4A820, 0XB64C2B00, 0X6D3D2D40, 0X8085AE60, 0X3667A2E0, 0XDBDF21C0, 0X00AE2780, 0XED16A4A0,
		0X80D2BDA0, 0X6D6A3E80, 0XB61B38C0, 0X5BA3BBE0, 0XED41B760, 0X00F93440, 0XDB883200, 0X3630B120,
		0XB7E95040, 0X5A51D360, 0X8120D520, 0X6C985600, 0XDA7A5A80, 0X37C2D9A0, 0XECB3DFE0, 0X010B5CC0,
		0X6CCF45C0, 0X8177C6E0, 0X5A06C0A0, 0XB7BE4380, 0X015C4F00, 0XECE4CC20, 0X3795CA60, 0XDA2D4940,
		0XEC1DF860, 0X01A57B40, 0XDAD47D00, 0X376CFE20, 0X818EF2A0, 0X6C367180, 0XB74777C0, 0X5AFFF4E0,
		0X373BEDE0, 0XDA836EC0, 0X01F26880, 0XEC4AEBA0, 0X5AA8E720, 0XB7106400, 0X6C616240, 0X81D9E160,
		0X826A23A0, 0X6FD2A080, 0XB4A3A6C0, 0X591B25E0, 0XEFF92960, 0X0241AA40, 0XD930AC00, 0X34882F20,
		0X594C3620, 0XB4F4B500, 0X6F85B340, 0X823D3060, 0X34DF3CE0, 0XD967BFC0, 0X0216B980, 0XEFAE3AA0,
		0XD99E8B80, 0X342608A0, 0XEF570EE0, 0X02EF8DC0, 0XB40D8140, 0X59B50260, 0X82C40420, 0X6F7C8700,
		0X02B89E00, 0XEF001D20, 0X34711B60, 0XD9C99840, 0X6F2B94C0, 0X829317E0, 0X59E211A0, 0XB45A9280,
		0X358373E0, 0XD83BF0C0, 0X034AF680, 0XEEF275A0, 0X58107920, 0XB5A8FA00, 0X6ED9FC40, 0X83617F60,
		0XEEA56660, 0X031DE540, 0XD86CE300, 0X35D46020, 0X83366CA0, 0X6E8EEF80, 0XB5FFE9C0, 0X58476AE0,
		0X6E77DBC0, 0X83CF58E0, 0X58BE5EA0, 0XB506DD80, 0X03E4D100, 0XEE5C5220, 0X352D5460, 0XD895D740,
		0XB551CE40, 0X58E94D60, 0X83984B20, 0X6E20C800, 0XD8C2C480, 0X357A47A0, 0XEE0B41E0, 0X03B3C2C0,
		0XE96CC460, 0X04D44740, 0XDFA54100, 0X321DC220, 0X84FFCEA0, 0X69474D80, 0XB2364BC0, 0X5F8EC8E0,
		0X324AD1E0, 0XDFF252C0, 0X04835480, 0XE93BD7A0, 0X5FD9DB20, 0XB2615800, 0X69105E40, 0X84A8DD60,
		0XB2986C40, 0X5F20EF60, 0X8451E920, 0X69E96A00, 0XDF0B6680, 0X32B3E5A0, 0XE9C2E3E0, 0X047A60C0,
		0X69BE79C0, 0X8406FAE0, 0X5F77FCA0, 0XB2CF7F80, 0X042D7300, 0XE995F020, 0X32E4F660, 0XDF5C7540,
		0X5E859420, 0XB33D1700, 0X684C1140, 0X85F49260, 0X33169EE0, 0XDEAE1DC0, 0X05DF1B80, 0XE86798A0,
		0X85A381A0, 0X681B0280, 0XB36A04C0, 0X5ED287E0, 0XE8308B60, 0X05880840, 0XDEF90E00, 0X33418D20,
		0X05713C00, 0XE8C9BF20, 0X33B8B960, 0XDE003A40, 0X68E236C0, 0X855AB5E0, 0X5E2BB3A0, 0XB3933080,
		0XDE572980, 0X33EFAAA0, 0XE89EACE0, 0X05262FC0, 0XB3C42340, 0X5E7CA060, 0X850DA620, 0X68B52500,
		0X6B06E7C0, 0X86BE64E0, 0X5DCF62A0, 0XB077E180, 0X0695ED00, 0XEB2D6E20, 0X305C6860, 0XDDE4EB40,
		0XB020F240, 0X5D987160, 0X86E97720, 0X6B51F400, 0XDDB3F880, 0X300B7BA0, 0XEB7A7DE0, 0X06C2FEC0,
		0X30F24FE0, 0XDD4ACCC0, 0X063BCA80, 0XEB8349A0, 0X5D614520, 0XB0D9C600, 0X6BA8C040, 0X86104360,
		0XEBD45A60, 0X066CD940, 0XDD1DDF00, 0X30A55C20, 0X864750A0, 0X6BFFD380, 0XB08ED5C0, 0X5D3656E0,
		0XDCEFB780, 0X315734A0, 0XEA2632E0, 0X079EB1C0, 0XB17CBD40, 0X5CC43E60, 0X87B53820, 0X6A0DBB00,
		0X07C9A200, 0XEA712120, 0X31002760, 0XDCB8A440, 0X6A5AA8C0, 0X87E22BE0, 0X5C932DA0, 0XB12BAE80,
		0X871B1FA0, 0X6AA39C80, 0XB1D29AC0, 0X5C6A19E0, 0XEA881560, 0X07309640, 0XDC419000, 0X31F91320,
		0X5C3D0A20, 0XB1858900, 0X6AF48F40, 0X874C0C60, 0X31AE00E0, 0XDC1683C0, 0X07678580, 0XEADF06A0
	};


	uint32 crc_ = checksum;
	uint8* pSrc = (uint8 *)src;
	while (len--)  crc_ = (crc_ << 8) ^ crctable[(crc_ >> 24) ^ *pSrc++];
	return crc_;
}

uint16 crc_16(const void* src, uint32 len, uint16 checksum)
{
	static uint8 auchCRCHi[]={
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
		0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
		0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
		0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
		0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
		0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
		0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
		0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
		0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
		0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
		0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
		0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
		0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
	}; 

	static uint8 auchCRCLo[]={
		0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 
		0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 
		0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 
		0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 
		0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4, 
		0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 
		0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 
		0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 
		0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 
		0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 
		0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 
		0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 
		0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 
		0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 
		0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 
		0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 
		0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 
		0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5, 
		0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 
		0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 
		0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 
		0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 
		0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 
		0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 
		0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 
		0x43, 0x83, 0x41, 0x81, 0x80, 0x40 
	}; 

	uint8 uchCRCHi = 0xFF ; 
	uint8 uchCRCLo = 0xFF ; 
	uint8 uIndex = 0; 
	uint8* pSrc = (uint8 *)src;
	while (len--)
	{ 
		uIndex = uchCRCHi ^ *pSrc++; 
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex]; 
		uchCRCLo = auchCRCLo[uIndex];
	}
	return (uchCRCHi << 8 | uchCRCLo);
}

uint16 crc_ccitt(const void* src, uint32 len, uint16 checksum)
{
	// 表的生成算法
	/*uint16 table[256];
	for (uint32 i = 0; i < 256; ++i)
		for (uint16 reg = i << 8, j = 0; j < 8; ++j)
			table[i] = reg = (reg & 0x8000) ? ( reg << 1) ^ 0X1021 : (reg << 1);*/

	static uint32 crctable[256] = {          // X16+X12+X5+1  余式表(0X1021)
		0X0000, 0X1021, 0X2042, 0X3063, 0X4084, 0X50A5, 0X60C6, 0X70E7,
		0X8108, 0X9129, 0XA14A, 0XB16B, 0XC18C, 0XD1AD, 0XE1CE, 0XF1EF,
		0X1231, 0X0210, 0X3273, 0X2252, 0X52B5, 0X4294, 0X72F7, 0X62D6,
		0X9339, 0X8318, 0XB37B, 0XA35A, 0XD3BD, 0XC39C, 0XF3FF, 0XE3DE,
		0X2462, 0X3443, 0X0420, 0X1401, 0X64E6, 0X74C7, 0X44A4, 0X5485,
		0XA56A, 0XB54B, 0X8528, 0X9509, 0XE5EE, 0XF5CF, 0XC5AC, 0XD58D,
		0X3653, 0X2672, 0X1611, 0X0630, 0X76D7, 0X66F6, 0X5695, 0X46B4,
		0XB75B, 0XA77A, 0X9719, 0X8738, 0XF7DF, 0XE7FE, 0XD79D, 0XC7BC,
		0X48C4, 0X58E5, 0X6886, 0X78A7, 0X0840, 0X1861, 0X2802, 0X3823,
		0XC9CC, 0XD9ED, 0XE98E, 0XF9AF, 0X8948, 0X9969, 0XA90A, 0XB92B,
		0X5AF5, 0X4AD4, 0X7AB7, 0X6A96, 0X1A71, 0X0A50, 0X3A33, 0X2A12,
		0XDBFD, 0XCBDC, 0XFBBF, 0XEB9E, 0X9B79, 0X8B58, 0XBB3B, 0XAB1A,
		0X6CA6, 0X7C87, 0X4CE4, 0X5CC5, 0X2C22, 0X3C03, 0X0C60, 0X1C41,
		0XEDAE, 0XFD8F, 0XCDEC, 0XDDCD, 0XAD2A, 0XBD0B, 0X8D68, 0X9D49,
		0X7E97, 0X6EB6, 0X5ED5, 0X4EF4, 0X3E13, 0X2E32, 0X1E51, 0X0E70,
		0XFF9F, 0XEFBE, 0XDFDD, 0XCFFC, 0XBF1B, 0XAF3A, 0X9F59, 0X8F78,
		0X9188, 0X81A9, 0XB1CA, 0XA1EB, 0XD10C, 0XC12D, 0XF14E, 0XE16F,
		0X1080, 0X00A1, 0X30C2, 0X20E3, 0X5004, 0X4025, 0X7046, 0X6067,
		0X83B9, 0X9398, 0XA3FB, 0XB3DA, 0XC33D, 0XD31C, 0XE37F, 0XF35E,
		0X02B1, 0X1290, 0X22F3, 0X32D2, 0X4235, 0X5214, 0X6277, 0X7256,
		0XB5EA, 0XA5CB, 0X95A8, 0X8589, 0XF56E, 0XE54F, 0XD52C, 0XC50D,
		0X34E2, 0X24C3, 0X14A0, 0X0481, 0X7466, 0X6447, 0X5424, 0X4405,
		0XA7DB, 0XB7FA, 0X8799, 0X97B8, 0XE75F, 0XF77E, 0XC71D, 0XD73C,
		0X26D3, 0X36F2, 0X0691, 0X16B0, 0X6657, 0X7676, 0X4615, 0X5634,
		0XD94C, 0XC96D, 0XF90E, 0XE92F, 0X99C8, 0X89E9, 0XB98A, 0XA9AB,
		0X5844, 0X4865, 0X7806, 0X6827, 0X18C0, 0X08E1, 0X3882, 0X28A3,
		0XCB7D, 0XDB5C, 0XEB3F, 0XFB1E, 0X8BF9, 0X9BD8, 0XABBB, 0XBB9A,
		0X4A75, 0X5A54, 0X6A37, 0X7A16, 0X0AF1, 0X1AD0, 0X2AB3, 0X3A92,
		0XFD2E, 0XED0F, 0XDD6C, 0XCD4D, 0XBDAA, 0XAD8B, 0X9DE8, 0X8DC9,
		0X7C26, 0X6C07, 0X5C64, 0X4C45, 0X3CA2, 0X2C83, 0X1CE0, 0X0CC1,
		0XEF1F, 0XFF3E, 0XCF5D, 0XDF7C, 0XAF9B, 0XBFBA, 0X8FD9, 0X9FF8,
		0X6E17, 0X7E36, 0X4E55, 0X5E74, 0X2E93, 0X3EB2, 0X0ED1, 0X1EF0
	};

	uint16 crc_ = checksum;
	uint8* pSrc = (uint8 *)src;
	while (len--)  crc_ = (crc_ << 8) ^ crctable[(crc_ >> 8) ^ *pSrc++];
	return crc_;
}

uint16 crc_ccitt_rev(const void* src, uint32 len, uint16 checksum)
{
	// 表的构造算法
	/*uint16 table[256];
	for (uint32 i = 0; i < 256; ++i)
		for (uint16 reg = i << 8, j = 0; j < 8; ++j)
			table[i] = reg = (reg & 0x8000) ? ( reg << 1) ^ 0x8408 : (reg << 1);*/

	static uint32 crctable[256] = {          // X16+X12+X5+1  余式表 (0X8408)
		0X0000, 0X8408, 0X8C18, 0X0810, 0X9C38, 0X1830, 0X1020, 0X9428,
		0XBC78, 0X3870, 0X3060, 0XB468, 0X2040, 0XA448, 0XAC58, 0X2850,
		0XFCF8, 0X78F0, 0X70E0, 0XF4E8, 0X60C0, 0XE4C8, 0XECD8, 0X68D0,
		0X4080, 0XC488, 0XCC98, 0X4890, 0XDCB8, 0X58B0, 0X50A0, 0XD4A8,
		0X7DF8, 0XF9F0, 0XF1E0, 0X75E8, 0XE1C0, 0X65C8, 0X6DD8, 0XE9D0,
		0XC180, 0X4588, 0X4D98, 0XC990, 0X5DB8, 0XD9B0, 0XD1A0, 0X55A8,
		0X8100, 0X0508, 0X0D18, 0X8910, 0X1D38, 0X9930, 0X9120, 0X1528,
		0X3D78, 0XB970, 0XB160, 0X3568, 0XA140, 0X2548, 0X2D58, 0XA950,
		0XFBF0, 0X7FF8, 0X77E8, 0XF3E0, 0X67C8, 0XE3C0, 0XEBD0, 0X6FD8,
		0X4788, 0XC380, 0XCB90, 0X4F98, 0XDBB0, 0X5FB8, 0X57A8, 0XD3A0,
		0X0708, 0X8300, 0X8B10, 0X0F18, 0X9B30, 0X1F38, 0X1728, 0X9320,
		0XBB70, 0X3F78, 0X3768, 0XB360, 0X2748, 0XA340, 0XAB50, 0X2F58,
		0X8608, 0X0200, 0X0A10, 0X8E18, 0X1A30, 0X9E38, 0X9628, 0X1220,
		0X3A70, 0XBE78, 0XB668, 0X3260, 0XA648, 0X2240, 0X2A50, 0XAE58,
		0X7AF0, 0XFEF8, 0XF6E8, 0X72E0, 0XE6C8, 0X62C0, 0X6AD0, 0XEED8,
		0XC688, 0X4280, 0X4A90, 0XCE98, 0X5AB0, 0XDEB8, 0XD6A8, 0X52A0,
		0X73E8, 0XF7E0, 0XFFF0, 0X7BF8, 0XEFD0, 0X6BD8, 0X63C8, 0XE7C0,
		0XCF90, 0X4B98, 0X4388, 0XC780, 0X53A8, 0XD7A0, 0XDFB0, 0X5BB8,
		0X8F10, 0X0B18, 0X0308, 0X8700, 0X1328, 0X9720, 0X9F30, 0X1B38,
		0X3368, 0XB760, 0XBF70, 0X3B78, 0XAF50, 0X2B58, 0X2348, 0XA740,
		0X0E10, 0X8A18, 0X8208, 0X0600, 0X9228, 0X1620, 0X1E30, 0X9A38,
		0XB268, 0X3660, 0X3E70, 0XBA78, 0X2E50, 0XAA58, 0XA248, 0X2640,
		0XF2E8, 0X76E0, 0X7EF0, 0XFAF8, 0X6ED0, 0XEAD8, 0XE2C8, 0X66C0,
		0X4E90, 0XCA98, 0XC288, 0X4680, 0XD2A8, 0X56A0, 0X5EB0, 0XDAB8,
		0X8818, 0X0C10, 0X0400, 0X8008, 0X1420, 0X9028, 0X9838, 0X1C30,
		0X3460, 0XB068, 0XB878, 0X3C70, 0XA858, 0X2C50, 0X2440, 0XA048,
		0X74E0, 0XF0E8, 0XF8F8, 0X7CF0, 0XE8D8, 0X6CD0, 0X64C0, 0XE0C8,
		0XC898, 0X4C90, 0X4480, 0XC088, 0X54A0, 0XD0A8, 0XD8B8, 0X5CB0,
		0XF5E0, 0X71E8, 0X79F8, 0XFDF0, 0X69D8, 0XEDD0, 0XE5C0, 0X61C8,
		0X4998, 0XCD90, 0XC580, 0X4188, 0XD5A0, 0X51A8, 0X59B8, 0XDDB0,
		0X0918, 0X8D10, 0X8500, 0X0108, 0X9520, 0X1128, 0X1938, 0X9D30,
		0XB560, 0X3168, 0X3978, 0XBD70, 0X2958, 0XAD50, 0XA540, 0X2148
	};

	uint16 crc_ = checksum;
	uint8* pSrc = (uint8 *)src;
	while (len--)  crc_ = crctable[(crc_ ^ *pSrc++) & 0XFF] ^ (crc_ >> 8);
	return crc_;
}


uint32 crc_32_checksum(const void* src, uint32 len, uint32 checksum)
{
	static const uint32 crc32_table[] = {
		0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L,
		0x706af48fL, 0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L,
		0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L,
		0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
		0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L,
		0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L,
		0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L,
		0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
		0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L,
		0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL,
		0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L,
		0xb8bda50fL, 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
		0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L,
		0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL,
		0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL,
		0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
		0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL,
		0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L,
		0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L,
		0xfbd44c65L, 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
		0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL,
		0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L,
		0xaa0a4c5fL, 0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L,
		0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
		0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L,
		0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L,
		0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L,
		0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
		0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L,
		0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL,
		0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL,
		0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
		0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L,
		0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL,
		0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL,
		0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
		0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL,
		0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L,
		0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL,
		0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
		0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL,
		0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L,
		0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L,
		0x18b74777L, 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
		0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L,
		0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L,
		0x4969474dL, 0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L,
		0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
		0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L,
		0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L,
		0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL,
		0x2d02ef8dL
	};

	uint32 crc_ = checksum;
	const uint8* buf = (const uint8*)src;
	const uint8 *end = buf + len;
	for(const uint8 *p = buf; p < end; ++p)
	{
		crc_ = crc32_table[(crc_ ^ *p) & 0xFF] ^ (crc_ >> 8);
	}
	return crc_;
}

} // namespace xcore


////////////////////////////////////////////////////////////////////////////////
// test section
////////////////////////////////////////////////////////////////////////////////

#ifdef _XCORE_NEED_TEST

#include "xcore_test.h"

namespace xcore
{

bool xcore_test_crc()
{
	//uint32 checksum32 = 0;
	uint16 checksum16 = 0;

	checksum16 = crc_ccitt("123", 3, checksum16);
	checksum16 = crc_ccitt("456", 3, checksum16);
	ASSERT(checksum16 == 0X20E4);
	checksum16 = 0;

// 	checksum16 = crc_16("123456", 6);
// 	ASSERT(checksum16 == 0X20E4);

	/*checksum32 = crc_32("123456", 6);
	ASSERT(checksum32 == 0X0972D361);*/
	return true;
}

}//namespace xcore

#endif//_XCORE_NEED_TEST
