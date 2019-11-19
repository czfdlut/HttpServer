////////////////////////////////////////////////////////////////////////////////////
// 
// @file: string_util.h
// @author: czfdlut@163.com
// @date: 2014/5/1	19:48
// @brief: �ַ���������
//          
////////////////////////////////////////////////////////////////////////////////////
#pragma once

#ifndef __STRUTIL_H_
#define __STRUTIL_H_

#include "type.h"
#include <vector>
#include <string>
#include <stdarg.h>

using std::string;
using std::vector;

namespace str_util {

	//�ַ�������
	//char* strcpy(char* pDestination, const char*  pSource);		
	char* strncpy(char* pDestination, const char* pSource, uint length);	
	// ע�⣡destSizeInByteΪĿ�Ļ�������sizeof������strlen
	char* strcpy_s(char* pDestination, const char* pSource, const int maxlen);

	// �ַ������
	char* strcat(char* pDestination, const char* pSource);				
	char* strncat(char* pDestination, const char* pSource, uint length);	

	// �ַ����Ƚ�
	int strcmp(const char* pSource1, const char* pSource2);								
	int strncmp(const char* pSource1, const char*  pSource2, uint length);	

	// �����ַ�������
	int strlen(const char* pSource);	
	int strUtf8len(const char* pSource);

	// �����ַ���
	const char* strstr(const char* pHaystack, const char* pNeedle);

	//��ʽ���ַ���
	int snprintf(char *buf, int maxlen, const char *format, ...);
	int vsnprintf(char* buf, int maxlen, const char* format, va_list varList);

	// �ֽ��ַ���
	char* strtok(char* pSource, const char* pDelimit);		
	char* strtok_s(char*  pSource, const char* pDelimit, char** pContext);
	// �з��ַ���
	int	 strToVector(const char* strSource, vector<string>& vRet, 
						   const char* strKey="\\/\n", bool bOneOfKey = true, bool bIgnoreEmpty = true);
	bool split(const string& str, char delim, vector<string>& r);

	// ��buffer�н�����һ��
	const char* getLineFromMemory(char* pStringBuf, int nBufSize, 
		const char* pMemStart, const char* pMemEnd);

	//�ַ���ת����
	int strToInt(const char* pSource);
    //�ַ���ת�޷�������
    uint strToUint(const char*  pSource, char**  pEnd = NULL, int nBase = 10);
	//�ַ���ת64λ����
	int64 strToInt64(const char*  pSource, char** pEnd = NULL, int nBase = 10);
	//�ַ���ת64λ�޷�������
    uint64 strToUint64(const char*  pSource, char** pEnd = NULL, int nBase = 10);
	//�ַ���ת������
	long strToL(const char*  pSource, char** pEnd = NULL, int nBase = 10);
	//�ַ���ת�޷��ų�����
	ulong strToUL(const char*  pSource, char** pEnd = NULL, int nBase = 10);
	//�ַ���ת������
	float strToFloat(const char* pSource);
	//�ַ���ת������
	double strToDouble(const char* pSource, char** pEnd);

	//����ת�ַ���
	char* int32ToStr(int nValue, char* pString, int nRadix);
	bool uint64ToStr(uint64 i, char* pDest, int base = 10);
	bool floatToStr(float i, char* pDest, int base = 10);

	// ȥ���ַ����пո��tab
	int	 trimRight(char* s);
	bool trimLeft(char* pSource);						
	bool trimRightV2(char* pSource);
	bool trim(char* pSource);
    // ɾ���ַ���str����β�ַ�c
    bool trim(string& str, char c) ;

	//�ж��ַ����Ƿ�����
	bool isInteger(const char* pSource);
	bool isFloat(const char* pSource);

	//����
	void toUpper(std::string &str);
	void toLower(std::string &str);
	std::string bin2hex(const void *data, unsigned int len);
	bool hex2bin(std::string &hex_str, unsigned char *&data, unsigned int len);
    string bytesToHexString(const unsigned char* input, size_t length);

    //����ת��
    bool gbkToUtf8(const char *src, string& result);
    bool utf8ToGbk(const char *src, string& result);

} // namespace str_util
using namespace str_util;

#endif	// __STRUTIL_H_
