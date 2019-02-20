////////////////////////////////////////////////////////////////////////////////////
// 
// @file: maths.h
// @author: by Mr.Chen
// @date: 2014/5/9	17:54
// @brief: ��ѧ�ຯ��
//
////////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef __MATHS_H_
#define __MATHS_H_

#include "type.h"
#include <iostream>
#include <time.h>
#include <string>
#include <stdlib.h>

using std::string;

namespace algorithm {

    const float PI				 = 3.1415926535897932384626433832795f;
    const float PI2			 = 6.283185307179586476925286766559f;
    const float PI_HALF		 = 1.5707963267948966192313216916398f;
    const float PI_QUATER	 = 0.78539816339744830961566084581988f;
	const double EPSILON	 = 1e-9; // ����˵������1e-9���ڵ����

	//�����ֵ
	template<typename T> 
	inline T			absolute(T x) { return x > 0 ? x : -x; }

	//��������ֵ
	template<typename T> 
	inline void		swap(T& a, T& b)
	{
		if (&a == &b) return;
		T temp = a; a = b; b = temp;
	}

	//�Ƕ�תΪ����
	inline float		radian(const float a) { return PI * a / 180;}

	//����תΪ�Ƕ�
	inline float		angle(const float r) { return 180 * r / PI; }

	//�жϸ������Ƿ����
	inline bool		floatEqual(const float a, const float b) { return absolute(a - b) <= EPSILON;}

	//Բ��������Ϊ����
	inline int		round(const float f)
	{
		if (f > 0) {return static_cast<int>(f + 0.5f);} 
		else { return -static_cast<int>(-f + 0.5); }
	}

	//�ж�һ��int�Ƿ�Ϊ����
	bool	isPrime(uint n);

	//�ҳ�С��n���������
	uint	minPrime(uint n);

	//��base��times�η�
	int		pow(int base, int times);

	//�����
	uint	rand_uint();
	uint	get_rand(int nStart, int nEnd);
	std::string	get_randstr(int length, char* prefix, bool letters, 
		bool numbers, bool symbols);
	int		select_rand_weight(int* weight, int n);

} //namespace algorithm
using namespace algorithm;


#endif	// __MATH_H_

