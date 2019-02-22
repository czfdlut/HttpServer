////////////////////////////////////////////////////////////////////////////////////
// 
// @file: time_system.h
// @author: czfdlut@163.com
// @date: 2014/8/3	20:44
// @brief: ʱ���������
//
////////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef __TIME_SYSTEM_H_
#define __TIME_SYSTEM_H_

#include <time.h>
#include <string>
#include "type.h"
#include "singleton.h"

#ifdef _LINUX64
#include <sys/utsname.h>
#include <sys/time.h>
#include <unistd.h>
#endif

namespace sys_util {

class TimeSystem : public Singleton<TimeSystem>
{
public:
	void	start();			//��ʼ��ʱ
	void  tick();

	// ȡ�ý�����ϸʱ�䣨������ʱ�������ڣ�
	int		getYear()			{ return (_tm.tm_year + 1900); };	//��
	int		getMonth()		{ return (_tm.tm_mon + 1); };		//��
	int		getDay()			{ return (_tm.tm_mday); };			//��
	int		getHour()			{ return (_tm.tm_hour); };				//ʱ
	int		getMinute()		{ return (_tm.tm_min); };				//��
	int		getSecs()			{ return (_tm.tm_sec); };				//��
	int		getWeek()		    { return (_tm.tm_wday); };			//���ڣ�0��ʾ������

public:
	uint	getToday();								//��ȡ��ǰ���ڣ���ȷ���죬���磺20120621 ��ʾ2012��6��21�ţ�
    uint	getCurrHourMinute();				    //��ȡ��ǰ���ڣ���ȷ�����ӣ����磺1206211930 ��ʾ12��6��21��19��룩
    int		getYearDay();								 //��õ�ǰ���ڣ���+��ǰ���������磺12001��ʾ2012��1��1�գ�
    int		getYearDay(int year, int month, int day);//�����ڸ�ʽ�������գ�ת�������ڸ�ʽ����+��ǰ�����������磺20120101ת��12001
	
	bool	isLeap(int year) const;						//�Ƿ�����
	uint	getYesterday();								//����������ڣ����磺20120620��
	int		getBeforeNDays(int date, int n) const;	 //��ȡǰn�죨���磺20120621-2, ���� 20120619��
	int		getAfterNDays(int date, int n) const;	 //��ȡ��n�죨���磺20120619+2, ���� 20120621��

	int		getDayDistance(int date1, int date2) const;  //����2�����ڲ����:20120606 - 20120605, ����1��
	int		getYearDayDistance(int x1, int x2) const; //���㣨��+��ǰ��������ʽ��ʾ����������֮���������

public:
	uint		getRunTime();	//��÷���������ʱ��
    uint64	getSystemTime();
    uint64	getTimeMicroSecs(); // get a 64 bit time
    uint64	getTimeMilliSecs(); // in ms
    uint64	getTimeSecs(); // in secs
    uint32	getTimeSecs32();

	void		getTimeTM();	//�����ʽ��ʱ��
    void		getTimestamp(char* datebuf, int size);
    void		getTimestamp(char* datebuf, int size, uint64 ms);
    std::string		getCurrDayTimeStr(const time_t *timer);
    std::string		getCurrDayStr(const time_t *timer);

	void sleep(uint64 ms);

private:
	uint64 _microSecs;		//��1970�����ڵ�΢����
	tm _tm;						//��ʽ��ʱ��

#if defined(_WIN32)
	uint _startTime;			//��������ʱ��

#elif defined(_LINUX64)
	//struct timeval _startTime;
	//struct timezone _tz;
	struct timespec _startTime;

#endif

};

} // namespace sys_util
using namespace sys_util;

#endif // __TIME_SYSTEM_H_
