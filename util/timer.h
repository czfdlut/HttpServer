////////////////////////////////////////////////////////////////////////////////////
// 
// @file: timer.h
// @author: czfdlut@163.com
// @date: 2014/8/3	18:39
// @brief: ��ʱ��
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef __TIMER_H_
#define __TIMER_H_

#include "type.h"

namespace sys_util {

//��ʱ��
class Timer
{
public:
	Timer();

	bool	isSet() const { return _isSet; }				//�Ƿ���Ч
	uint	getStartTime() const { return _startTime; }	//��ÿ�ʼʱ��
	void	setTerm(uint nTerm){ _term = nTerm; }		//������Чʱ��
	uint	getTerm() const { return _term; }				//�����Чʱ��
		
	bool	beginTimer(uint nTerm, uint nNowTime);	//��ʼ��ʱ
	void	endTimer();											//������ʱ
	bool	isReach(uint uNowTime) ;				//�Ƿ�ʱ�䵽

	Timer& operator = (const Timer& rhs)
	{
		_startTime	= rhs.getStartTime();
		_term		= rhs.getTerm();
		_isSet		= rhs.isSet();
		return *this;
	};

private:
	uint	_startTime;		//��ʼʱ��
	uint	_term;				//��Чʱ��
	bool	_isSet;				//�Ƿ���Ч
};

} // namespace sys_util
using namespace sys_util;

#endif // __TIMER_H_
