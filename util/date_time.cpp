#include <cstdlib>
#include <sys/time.h>
#include "date_time.h"
#include <string>
#include <string.h>

using namespace std;

namespace ns_date_time {

    double get_current_sec()
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return tv.tv_sec + (double)tv.tv_usec / 1000000.0f;
    }

    uint64_t DateTime::TimestampSecond()
    {
        return TimestampMicrosecond() / 1000000;
    }

    uint64_t DateTime::TimestampMillisecond()
    {
        return TimestampMicrosecond() / 1000;
    }

    uint64_t DateTime::TimestampMicrosecond()
    {
        struct timeval tv;
        timerclear(&tv);
        gettimeofday(&tv, NULL);

        return (tv.tv_sec * 1000000 + tv.tv_usec);
    }

    int getdifftime(struct timeval& st1, struct timeval& st2)
    {
        return (st2.tv_sec - st1.tv_sec) * 1000 + (st2.tv_usec - st1.tv_usec) / 1000;
    }

    int time_statistics(struct timeval& last_time, const string& query, const string& desc)
    {
        struct timeval st2; gettimeofday(&st2, NULL);
        int difftime = getdifftime(last_time, st2);
        //LOG(INFO) << query << "\ttime-statistics\t" << desc << "\t" << difftime;
        //qLogInfos("CloudSearch.Merger") << "Time-Statistics\t" << desc << "\t" << query << "\t" << "\t" << difftime;
        memcpy(&last_time, &st2, sizeof(st2));
        return difftime;
    }

    void Timer::TimerBegin(const string &key)
    {
        if (elapsed_time.find(key) == elapsed_time.end())
            elapsed_time[key] = pair<double, double>(0, 0);

        elapsed_time[key].first = get_current_sec();
        return;
    }

    void Timer::TimerEnd(const string &key)
    {
        if (elapsed_time.find(key) == elapsed_time.end())
            elapsed_time[key] = pair<double, double>(get_current_sec(), 0);

        elapsed_time[key].second = get_current_sec();
        return;
    }

    double Timer::ElapsedTime(const string& key)
    {
        if (elapsed_time.find(key) == elapsed_time.end())
            return 0;

        if (elapsed_time[key].second < elapsed_time[key].first)
            elapsed_time[key].second = get_current_sec();

        return ((elapsed_time[key].second - elapsed_time[key].first) * 1000);
    }


} //< namespace ns_date_time

