////////////////////////////////////////////////////////////////////////////////////
// 
// File: singleton.h
// Author: czfdlut@163.com
// CreateTime: 2014/5/1	2:32
// Desc: ����ģʽģ����(�̰߳�ȫ)
//          
////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef _SINGLETON_H
#define _SINGLETON_H

#include "lock.h"

namespace single_pattern {

//����ģ����
template<class T> 
class Singleton
{
public:
	static T* getSinglePtr()
	{
		static MLock s_lock;
		if (NULL == _s_pSingleton)  {
			AutoMLock lock(s_lock);
			if (NULL == _s_pSingleton) {
				_s_pSingleton = new T();
			}
		}
		return _s_pSingleton;
	}

protected:
	Singleton() {}

private:
	static T* _s_pSingleton;
};

//��Ա��ʼ����
#define WX_IMPLEMENT_SINGLEON(className)\
	template<> className* Singleton<className>::_s_pSingleton = NULL;

} //namespace single_pattern
using namespace single_pattern;

#endif	//__SINGLETON_H_
