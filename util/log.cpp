#ifdef _WIN32
#include "socketdef.h"
#endif
#include "log.h"
#include "time_system.h"
#include "string_util.h"
#include <stdlib.h>
#include <stdarg.h>
#include "lock.h"
#include "maths.h"
#include "myassert.h"
#include "macrodef.h"
#include "helper.h"
#include <string.h>
#include <stdio.h>

WX_IMPLEMENT_SINGLEON(LogSystem);

char* g_logFileName[LOG_FILE_NUMBER] = {0};

bool LogSystem::init(int cachesize /*= DEFAULT_LOG_CACHE_SIZE*/)
{
	__ENTER_FUNCTION

    g_logFileName[LOG_FILE_DEBUG] = "./log/Debug";
    g_logFileName[LOG_FILE_WARN] = "./log/Warn";
    g_logFileName[LOG_FILE_INFO] = "./log/Info";
    g_logFileName[LOG_FILE_FATAL] = "./log/Fatal";
    g_logFileName[LOG_FILE_ERROR] = "./log/Error";

	if (cachesize > 0) { _cacheSize = cachesize; }
	else { _cacheSize = DEFAULT_LOG_CACHE_SIZE; }
	for (int i = 0; i < LOG_FILE_NUMBER; i++)
	{
        _logCache[i] = new char[_cacheSize];
		if (!_logCache[i]) { return false; }
		memset(_logCache[i], 0, _cacheSize);
		_logPos[i] = 0;
	}
	_pLogSaveDisk = new char[_cacheSize];
	if (!_pLogSaveDisk) {return false; }
	memset(_pLogSaveDisk, 0, _cacheSize);
	return true ;
	__LEAVE_FUNCTION
	return false ;
}

bool LogSystem::saveLog(LOG_FILE logid, char* msg, ...)
{
	__ENTER_FUNCTION
    if (logid < 0 || logid >= LOG_FILE_NUMBER)
	{
		return false;
	}
    AutoMLock lock(_logLock[logid]);
    va_list argptr;
	va_start(argptr, msg);
    char buffer[MAX_SINGLE_LOG_LEN] = {0};
	str_util::vsnprintf(buffer, MAX_SINGLE_LOG_LEN, msg, argptr);
	va_end(argptr);
    if (TimeSystem::getSinglePtr())
	{
		char timestr[MAX_SINGLE_LOG_LEN] = {0};
		TimeSystem::getSinglePtr()->getTimestamp(timestr, sizeof(timestr) / sizeof(char));
		char temp[MAX_SINGLE_LOG_LEN] = {0} ;
		str_util::snprintf(temp, MAX_SINGLE_LOG_LEN, "[tid=%d][time=%s][%d]\r\n", 
			getCurrTid(), timestr, TimeSystem::getSinglePtr()->getRunTime());
        str_util::strcat(buffer, temp) ;
	}
    char filename[MAX_FILE_NAME_LEN] = {0};
    if (!getLogName(logid, filename, sizeof(filename)))
	{
		return false;
	}
	FILE* f = fopen(filename, "ab") ;
	if (f == NULL)
	{
        printf("log %s error\r\n", g_logFileName[logid]);
		MyAssertSpecial(0, "Open file %s fail.", filename);
        return false;
	}
	fwrite(buffer, 1, str_util::strlen(buffer), f);
	fclose(f);
	printf(buffer);

	return true;
	__LEAVE_FUNCTION
	return false;
}

bool LogSystem::getLogName(int logid, char* buffer, int len)
{
	__ENTER_FUNCTION
	if (logid < 0 || logid >= LOG_FILE_NUMBER)
	{
		return false;
	}
	if (buffer == NULL)
	{
		return false;
	}
	memset(buffer, 0, len) ;
    if(TimeSystem::getSinglePtr())
	{
		str_util::snprintf(buffer, len, 
			"%s_%.4d_%.2d_%.2d.log", 
			g_logFileName[logid],
			TimeSystem::getSinglePtr()->getYear(),
			TimeSystem::getSinglePtr()->getMonth(),
			TimeSystem::getSinglePtr()->getDay()) ;
	}
	return true;
	__LEAVE_FUNCTION
    return false;
}

void LogSystem::flushLog(int logid)
{
	__ENTER_FUNCTION
    char filename[MAX_FILE_NAME_LEN] = {0};
	getLogName(logid, filename, sizeof(filename)) ;

    _logLock[logid].lock();
    swap<char*>(_logCache[logid], _pLogSaveDisk);
    //char* temp = _logCache[logid];
    //_logCache[logid] = _pLogSaveDisk;
    //_pLogSaveDisk = temp;

	int needSaveSize = _logPos[logid] ;
	_logPos[logid] = 0;
	_logLock[logid].unlock() ;

	__MYTRY
	{
		if (needSaveSize == 0)
			return;
		FILE* f = fopen(filename, "ab") ;
		if (f)
		{
			__MYTRY
			{
				fwrite(_pLogSaveDisk, 1, needSaveSize, f) ;
			}
			__MYCATCH
			{
			}
			fclose(f);
		}
	}
	__MYCATCH
	{
	}
	__LEAVE_FUNCTION
}

void LogSystem::flushAll(LOG_FILE level)
{
	__ENTER_FUNCTION
	for (int i = 0; i < LOG_FILE_NUMBER; i++)
	{
        if (i >= level)
            flushLog(i);
	}
	__LEAVE_FUNCTION
}

//��־д�뻺��,�ٶȿ�
void LogSystem::cacheLog(int logid, char* msg, ...)
{
	__ENTER_FUNCTION
	if (logid < 0 || logid >= LOG_FILE_NUMBER)
	{
		return;
	}
    char buffer[MAX_SINGLE_LOG_LEN] = {0};
	va_list argptr;
	__MYTRY
	{
		va_start(argptr, msg);
		str_util::vsnprintf(buffer,MAX_SINGLE_LOG_LEN,msg,argptr);
		va_end(argptr);
        if (TimeSystem::getSinglePtr())
		{
			char timeBuf[MAX_SINGLE_LOG_LEN] = {0};
			TimeSystem::getSinglePtr()->getTimestamp(timeBuf, sizeof(timeBuf) / sizeof(char));
			char temp[MAX_SINGLE_LOG_LEN] = {0} ;
			str_util::snprintf(temp, MAX_SINGLE_LOG_LEN, "[tid=%d][time=%s][%d]\r\n", 
				getCurrTid(), timeBuf, TimeSystem::getSinglePtr()->getRunTime()) ;
			str_util::strcat(buffer, temp) ;
		}
	}
	__MYCATCH
	{
		return;
	}
	//�ն���ʾ
	printf(buffer);
	int iLen = (int)str_util::strlen(buffer) ;
	if (iLen <= 0)
		return;
	//����ط��첽�Ļ�����ҪԤ��һЩ�ֽڣ���������Խ��
	if (_logPos[logid] > (_cacheSize - iLen - 4096))
	{
		//�����Ҫ�洢����־�ռ��ʣ����־�ռ仹Ҫ��������־
		__MYTRY
		{
			char filename[256] = {0};
            str_util::snprintf(filename,256,"../Log/LogDiscard_%d_%d.log",rand(),rand());
			FILE* f = fopen(filename, "ab" ) ;
			if (f)
			{
				__MYTRY
				{
					char logMessage[256] = "log discard! \r\n";
					fwrite(logMessage, 1, str_util::strlen(logMessage), f) ;
				}
				__MYCATCH
				{
				}
				fclose(f);
			}
		}
		__MYCATCH
		{
		}
		return;
	}
    AutoMLock lock(_logLock[logid]);
	__MYTRY
	{
		memcpy(_logCache[logid] + _logPos[logid], buffer, iLen) ;
		_logPos[logid] += iLen ;
	}
	__MYCATCH
	{
	}
	__LEAVE_FUNCTION
}
