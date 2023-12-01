/*********************************************************************
 * \file   ktimer.h
 * \brief  定时器类，包括timer、sleep、elapsed功能
 *
 * \author jiang
 * \date   2023.06.15
 *********************************************************************/
 /// modify by kk 2020 4.26
#ifndef KTIMER_H
#define KTIMER_H

#include<condition_variable>
#include<mutex>
#include<thread>
#include<functional>
#include<atomic>
#include<map>
#include<memory>
using namespace std;
#include "ktime.h"
#include "ProducerConsumerTemplate.h"

template <class TimeUnit = std::chrono::milliseconds>
class KTimer
{
public:
	KTimer() = default;
	KTimer(KTimer& other) = delete;
	KTimer& operator = (KTimer& other) = delete;
	~KTimer()
	{
		CancelTimer();
		CancelSleep();
	}
	template <class callable, class... arguments>
	void StartTimer(int after, bool is_cycle, callable&& f, arguments&&... args)
	{
		typedef std::function<typename std::result_of<callable(arguments...)>::type()> FuncType;
		CancelTimer();
		m_timer_cancel = false;
		FuncType* ptask = new FuncType(std::bind(std::forward<callable>(f), std::forward<arguments>(args)...));
		m_timer_thread = std::thread([this, after, is_cycle, ptask]()
			{
				do
				{
					if (m_timer_cancel)
					{
						//printf("timer canceled\n");
						break;
					}
					m_timer_task.Open([](const shared_ptr<int*>& event)->int
						{
							//printf("timer task executing\n");
							FuncType* ptask = (FuncType*)(*event);
							//ptask();
							(*ptask)();
							//printf("timer task execute completed\n");
							return 0;
						});
					if (m_timer_cancel)
					{
						//printf("timer canceled\n");
						break;
					}
					std::cv_status status;
					unique_lock<mutex> lock(m_timer_mutex);
					status = m_timer_condition.wait_for(lock, TimeUnit(after));
					if (status == std::cv_status::no_timeout)
					{
						if (m_timer_cancel)
						{
							//printf("timer canceled\n");
							break;
						}
						else
						{
							//printf("timer exception regard as timeout\n");
							goto timeout;
						}
					}
					else if (status == std::cv_status::timeout)
					{
					timeout:
#if 0
						printf("timer arrived and task executing\n");
						std::thread([ptask]() { /*task()*/(*ptask)(); printf("timer task execute completed\n"); }).detach();
#else
						//printf("timer arrived and add task\n");
						int* temp_ptask = (int*)(ptask);
						shared_ptr <int*> event = std::make_shared<int*>(temp_ptask);
						m_timer_task.Producer(event);
#endif
					}
				} while (is_cycle);
			});
	}
	void CancelTimer()
	{
		//printf("cancel timer\n");
		m_timer_cancel = true;
		m_timer_condition.notify_all();
		if (m_timer_thread.joinable())
		{
			m_timer_thread.join();
		}
		m_timer_task.Close();
	}
	int Sleep(int after)
	{
		//printf("sleeping\n");
		m_sleep_cancel = false;
		std::cv_status status;
		unique_lock<mutex> lock(m_sleep_mutex);
		status = m_sleep_condition.wait_for(lock, TimeUnit(after));
		if (status == std::cv_status::no_timeout)
		{
			if (m_sleep_cancel)
			{
				//printf("sleep canceled\n");
			}
			else
			{
				//printf("sleep exception regard as timeout\n");
				goto timeout;
			}
		}
		else if (status == std::cv_status::timeout)
		{
		timeout:;
			//printf("sleep completed\n");
		}
		return 0;
	}
	int CancelSleep()
	{
		//printf("cancel sleep\n");
		m_sleep_cancel = true;
		m_sleep_condition.notify_all();
		return 0;
	}
	__int64 ClockStart()
	{
		m_clockstart_time = KTime<TimeUnit/*std::chrono::microseconds*/>::GetNowDateTime();
		return m_clockstart_time;
	}
	__int64 ClockElapsed()
	{
		return KTime<TimeUnit/*std::chrono::microseconds*/>::GetNowDateTime() - m_clockstart_time;
	}
	string ClockStartStr()
	{
		return KTime<TimeUnit>::GetDateTime(ClockStart());
	}
	string ClockElapsedStr()
	{
		return KTime<TimeUnit>::GetClockTime(ClockElapsed());
	}

private:
	mutex                 m_timer_mutex;
	condition_variable    m_timer_condition;
	atomic<bool>          m_timer_cancel;
	thread                m_timer_thread;
	mutex                 m_sleep_mutex;
	condition_variable    m_sleep_condition;
	atomic<bool>          m_sleep_cancel;
	time_t                m_clockstart_time;
	ProducerConsumerTemplate<int*> m_timer_task;
};

#endif // KTIMER_H
