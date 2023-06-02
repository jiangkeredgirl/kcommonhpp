#pragma once
/*********************************************************************
 * \file   ProducerConsumerTemplate.h
 * \brief  生产者消费者模式封装，调用者不用关心实现，只需处理数据即可
 *
 * \author 蒋珂
 * \date   2023.05.08
 *********************************************************************/
#include <vector>
#include <map>
#include <iostream>
#include <string>
using namespace std;


/// 生产者-消费者模式数据模板
template<class DataType>
class ProducerConsumerTemplate
{
public:
	typedef function<int(const shared_ptr<DataType>& data)> ConsumerF;

	/**
	 * 打开生产-消费线程，设置消费者回调函数. 
	 */
	int Open(ConsumerF consumerf)
	{
		m_consumer_callback = consumerf;
		ConsumerThreadRun();
		return 0;
	}
	/**
	 * 生产数据，调用此函数会将数据发送给消费者回调函数.
	 * \return 0 数据生产成功，1 数据生产失败 
	 */
	int Producer(const shared_ptr<DataType>& data)
	{
		int error_code = 1;
		if (m_consumer_thread.joinable())
		{
			{
				lock_guard<mutex> listLock(m_data_list_mutex);
				m_data_list.push_back(data);
			}
			m_consumer_condition.notify_one();
			error_code = 0;
		}
		return error_code;
	}
	/**
	 * 返回数据列表中数据个数. 
	 * \return 数据列表大小
	 */
	int GetDataListSize()
	{
		return m_data_list.size();
	}
	/**
     * 关闭生产-消费线程.
     * \return 0 
     */
	int Close()
	{
		m_consumer_thread_kill = true;
		m_consumer_condition.notify_all();
		if (m_consumer_thread.joinable())
		{
			m_consumer_thread.join();
		}
		return 0;
	}

private:
	int Consumer(const shared_ptr<DataType>& data)
	{
		if (m_consumer_callback)
		{
			m_consumer_callback(data);
		}
		return 0;
	}
	int ConsumerThreadRun()
	{
		do
		{
			if (m_consumer_thread.joinable())
			{
				break;
			}
			m_consumer_thread_kill = false;
			m_consumer_thread = thread([this]
				{
					while (true)
					{
						if (m_consumer_thread_kill)
						{
							break;
						}
						unique_lock<mutex> condition_lock(m_consumer_mutex);
						m_consumer_condition.wait(condition_lock, [this]()->bool {return (m_consumer_thread_kill || !m_data_list.empty()); });
						while (!m_data_list.empty())
						{
							shared_ptr<DataType> data;
							{
								lock_guard<mutex> lock(m_data_list_mutex);
								data = m_data_list.front();
								m_data_list.pop_front();
							}
							Consumer(data);
						}
					}
				});

		} while (false);
		return 0;
	}

private:
	list<shared_ptr<DataType>>        m_data_list;                    //< 数据列表
	mutex                             m_data_list_mutex;              //< 列表mutex，防止list crash
	thread                            m_consumer_thread;              //< 数据消费线程，异步处理事件      
	mutex                             m_consumer_mutex;               //< 线程mutex，配合condition用
	condition_variable                m_consumer_condition;           //< 线程condition，线程通知  
	bool                              m_consumer_thread_kill = false; //< 线程kill标记，退出程序前kill线程
	ConsumerF                         m_consumer_callback = nullptr;  //< 消费者回调函数
};


/**
 * 获得对象的类名模板. 
 */
template<class T>
std::string  ClassName(T& object)
{
	//返回的类的全名
	std::string className = std::string(typeid(object).name());
	//第一个空格索引
	int firstSpaceIndex = className.find_first_of(" ") + 1;
	//最后一个空格索引
	int lastSpaceIndex = className.find_last_of(" ");
	//类名长度
	int nameLength = lastSpaceIndex - firstSpaceIndex - 2;
	return className.substr(firstSpaceIndex, nameLength);
}