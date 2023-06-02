#pragma once

#include <string>
#include <memory>
#include <mutex>
#include <cinttypes>
#include <sstream>
using namespace std;


inline int64_t GetThreadId(const std::thread* thread = nullptr)
{
    int64_t thread_id = 0;
    if(thread == nullptr)
    {
        stringstream ss;
        ss << std::this_thread::get_id();
        thread_id =  stoll(ss.str());
    }
    else
    {
        stringstream ss;
        ss << thread->get_id();
        thread_id =  stol(ss.str());
    }
    return thread_id;
}
#if 0
inline int GetThreadPolicy(int& policy, int& priority, int64_t thread_id = 0)
{
    int error_code = 0;
    if(thread_id == 0)
    {
        stringstream ss;
        ss << std::this_thread::get_id();
        thread_id =  stoll(ss.str());
    }
    printf("GetThreadPolicy  thread_id = %lld\n", (long long)thread_id);

    int _policy = 0;
    sched_param _param;
    _param.__sched_priority = 0;
    error_code = pthread_getschedparam(thread_id, &_policy, &_param);
    if(error_code == 0)
    {
        policy = _policy;
        switch(policy)
        {
        case SCHED_OTHER:
        {
            printf("get policy = SCHED_OTHER\n");
            break;
        }
        case SCHED_FIFO:
        {
            printf("get policy = SCHED_FIFO\n");
            priority = _param.__sched_priority;
            printf("get priority = %d\n", priority);
            int priority_min = sched_get_priority_min(policy);
            printf("get priority_min = %d\n",priority_min);
            int priority_max = sched_get_priority_max(policy);
            printf("get priority_max = %d\n",priority_max);
            break;
        }
        case SCHED_RR:
        {
            printf("get policy = SCHED_RR\n");
            priority = _param.__sched_priority;
            printf("get priority = %d\n", priority);
            int priority_min = sched_get_priority_min(policy);
            printf("get priority_min = %d\n",priority_min);
            int priority_max = sched_get_priority_max(policy);
            printf("get priority_max = %d\n",priority_max);
            break;
        }
        default:
            printf("get policy = %d UNKNOWN\n", policy);
            break;
        }
    }

    return error_code;
}


inline int SetThreadPolicy(int policy, int priority, int64_t thread_id = 0)
{
    int error_code = 0;
    if(thread_id == 0)
    {
        stringstream ss;
        ss << std::this_thread::get_id();
        thread_id =  stoll(ss.str());
    }
    printf("SetThreadPolicy  thread_id = %lld\n", (long long)thread_id);

    sched_param _param;
    _param.__sched_priority = priority;
    error_code = pthread_setschedparam(thread_id, policy, &_param);
    if(error_code == 0)
    {
        switch(policy)
        {
        case SCHED_OTHER:
        {
            printf("set policy = SCHED_OTHER\n");
            break;
        }
        case SCHED_FIFO:
        {
            printf("set policy = SCHED_FIFO\n");
            printf("set priority = %d\n", priority);
            int priority_min = sched_get_priority_min(policy);
            printf("set priority_min = %d\n",priority_min);
            int priority_max = sched_get_priority_max(policy);
            printf("set priority_max = %d\n",priority_max);
            break;
        }
        case SCHED_RR:
        {
            printf("set policy = SCHED_RR\n");
            printf("set priority = %d\n", priority);
            int priority_min = sched_get_priority_min(policy);
            printf("set priority_min = %d\n",priority_min);
            int priority_max = sched_get_priority_max(policy);
            printf("set priority_max = %d\n",priority_max);
            break;
        }
        default:
            printf("set policy = %d UNKNOWN\n", policy);
            break;
        }
    }

    return error_code;
}
#endif
