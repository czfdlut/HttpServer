/******************************************************************************\
 *
 * @file        date_time.h
 * @brief
 * @author      iHuHoo.W (ihuhoo.w@gmail.com)
 * @date        2014-04-03 11:46:22
 *
 *     Copyright (C) 2014, iHuHoo.W, all rights reserved.
 *
 \******************************************************************************/
#ifndef __DATE_TIME_H__
#define __DATE_TIME_H__

#include <stdint.h>
#include <string>  
#include <sys/time.h>   
#include <map>
#include <string>

using std::map;
using std::string;

namespace cloud_search {

    class DateTime {
    public:
        static uint64_t TimestampSecond();
        static uint64_t TimestampMillisecond();
        static uint64_t TimestampMicrosecond();
    };

    int getdifftime(struct timeval& st1, struct timeval& st2);

    int time_statistics(struct timeval& last_time, const std::string& query, const std::string& desc);

    class Timer {
    private:
        std::map<std::string, std::pair<double, double> > elapsed_time;

    public:
        Timer() { }
        ~Timer() { }
        void TimerBegin(const std::string &key);
        void TimerEnd(const std::string &key);
        double ElapsedTime(const std::string& key);
    };


} //< namespace cloud_search

#endif /* __DATE_TIME_H__ */

