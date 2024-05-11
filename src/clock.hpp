#ifndef CLOCK_HPP
#define CLOCK_HPP
#include <Windows.h>
#include <ctime>
#include <string>

class Clock
{
public:
    typedef unsigned long long TimeValue;
public:

    static TimeValue getMillisecondTimestamp()
    {
        TimeValue timestamp;
        SYSTEMTIME currentTime;
        GetLocalTime(&currentTime);
        struct tm t = {currentTime.wSecond,
                       currentTime.wMinute,
                       currentTime.wHour,
                       currentTime.wDay,
                       currentTime.wMonth - 1,
                       currentTime.wYear - 1900};
        timestamp = mktime(&t)*1000 + currentTime.wMilliseconds;
        return timestamp;
    }

    static TimeValue getSecondTimestamp()
    {
        TimeValue timestamp;
        SYSTEMTIME currentTime;
        GetLocalTime(&currentTime);
        struct tm t = {currentTime.wSecond,
                       currentTime.wMinute,
                       currentTime.wHour,
                       currentTime.wDay,
                       currentTime.wMonth - 1,
                       currentTime.wYear - 1900};
        timestamp = mktime(&t);
        return timestamp;
    }

    static LARGE_INTEGER now()
    {
        LARGE_INTEGER t;
        QueryPerformanceCounter(&t);
        return t;
    }

    static double duration(const LARGE_INTEGER& t2, const LARGE_INTEGER& t1)
    {
        LARGE_INTEGER freq;
        QueryPerformanceFrequency(&freq);
        return ((double)(t2.QuadPart - t1.QuadPart))/double(freq.QuadPart);
    }

    static std::string currentTime()
    {
        char acYear[5] = {0};
        char acMonth[5] = {0};
        char acDay[5] = {0};
        char acHour[5] = {0};
        char acMin[5] = {0};
        char acSec[5] = {0};
        time_t now;
        struct tm* timenow;
        time(&now);
        timenow = localtime(&now);
        strftime(acYear,sizeof(acYear),"%Y",timenow);
        strftime(acMonth,sizeof(acMonth),"%m",timenow);
        strftime(acDay,sizeof(acDay),"%d",timenow);
        strftime(acHour,sizeof(acHour),"%H",timenow);
        strftime(acMin,sizeof(acMin),"%M",timenow);
        strftime(acSec,sizeof(acSec),"%S",timenow);
        char timeBuffer[64];
        sprintf(timeBuffer, "%s-%s-%s:%s:%s:%s", acYear, acMonth, acDay, acHour, acMin, acSec);
        return std::string(timeBuffer);
    }

    static std::string currentDate()
    {
        char acYear[5] = {0};
        char acMonth[5] = {0};
        char acDay[5] = {0};
        time_t now;
        struct tm* timenow;
        time(&now);
        timenow = localtime(&now);
        strftime(acYear,sizeof(acYear),"%Y",timenow);
        strftime(acMonth,sizeof(acMonth),"%m",timenow);
        strftime(acDay,sizeof(acDay),"%d",timenow);
        char timeBuffer[64];
        sprintf(timeBuffer, "%s-%s-%s", acYear, acMonth, acDay);
        return std::string(timeBuffer);
    }
};

#endif // CLOCK_HPP
