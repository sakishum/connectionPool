/**
 * @file connect_pool.cpp
 * @Synopsis  Connection Pool for MySQL with pThread.
 * @author Saki Shum, sakishum1118@gmail.com
 * @version 0.1.7
 * @date 2013-03-05
 */
#include <stdexcept> 
#include <exception>  
#include <stdio.h>  
#include "connect_pool.h"

using namespace std;
using namespace sql;

/// 全局变量 
const string g_sdefaultHost = "localhost";
const string g_sdefaultUser = "root";
const string g_sdefaultPass = "qwer1234";
const int g_imaxSize = 50;

/// 单例声明
auto_ptr<CConnctionPool> CConnctionPool::auto_ptr_instance;

/**
 * @Synopsis  CConnctionPool 
 *	连接池构造函数
 * @Param url
 * @Param userName
 * @Param password
 * @Param maxSize
 */
CConnctionPool::CConnctionPool(string url, string userName, string password, int maxSize)
:m_imaxSize(maxSize), m_icurSize(0), m_suserName(userName), m_spassword(password), m_surl(url)
{
	try {
		m_pdriver = get_driver_instance();
	}
	catch(sql::SQLException &e) {
		perror("驱动连接出错;\n");
	}
	/*catch(sql::runtime_error &e) {
		perror("运行出错了\n");
	}*/
	this->InitConnection(m_imaxSize/2);
}

/**
 * @Synopsis  ~CConnctionPool 
 *	连接池析构函数
 */
CConnctionPool::~CConnctionPool(void)
{
	this->TerminateConnectionPool();
}

/**
 * @Synopsis  GetInstance 
 *	获取连接池对象，单例模式
 * @Returns   连接池对象
 */
CConnctionPool& CConnctionPool::GetInstance(void)
{
	if (auto_ptr_instance.get() == 0) {
		auto_ptr_instance.reset(new CConnctionPool(g_sdefaultHost, g_sdefaultUser, g_sdefaultPass, g_imaxSize));
	}
	return *(auto_ptr_instance.get());
}

/**
 * @Synopsis  InitConnection 
 *	初始化连接池，创建最大连接数的一半连接量
 * @Param iInitialSize
 */
void CConnctionPool::InitConnection(int iInitialSize)
{
	Connection *pConn;
	pthread_mutex_lock(&lock);
	for (int i = 0; i < iInitialSize; ++i) {
		pConn = this->CreateConnection();
		if (pConn) {
			m_connlist.push_back(pConn);
			++(this->m_icurSize);
		} else {
			perror("创建Connection 出错!");
		}
	}
	pthread_mutex_unlock(&lock);
}

/**
 * @Synopsis  CreateConnection 
 *	创建连接，返回一个 Connection
 * @Returns   sql::connection*
 */
Connection* CConnctionPool::CreateConnection(void)
{
	Connection *pConn;
	try {
		pConn = m_pdriver->connect(this->m_surl, this->m_suserName, this->m_spassword);
		return pConn;
	} 
	catch(sql::SQLException &e) {
		perror("创建连接出现错误!");
		return NULL;
	}
	/*catch(sql::runtime_error  &e) {
		perror("运行时出现错误!");
		return NULL;
	}*/
}

/**
 * @Synopsis  GetConnection 
 *	在连接池中获取一个连接
 * @Returns   sql::connection*
 */
Connection* CConnctionPool::GetConnection(void)
{
	Connection *pConn;
	pthread_mutex_lock(&lock);
	/// 连接池容器中还有连接
	if (m_connlist.size() > 0) {
		pConn = m_connlist.front();		/// 得到第一个连接
		m_connlist.pop_front();			/// 移除第一个连接
		/// 如果连接池已经关闭，删除后重新建立一个
		if (pConn->isClosed()) {
			delete pConn;
			pConn = this->CreateConnection();
		}
		/// 如果连接为空，则创建连接出错
		if (NULL == pConn) {
			--m_icurSize;
		}
		pthread_mutex_unlock(&lock);
		return pConn;
	} else {
		/// 还可以创建新的连接
		if (m_icurSize < m_imaxSize) {
			pConn = this->CreateConnection();
			if (pConn) {
				++m_icurSize;
				pthread_mutex_unlock(&lock); 
				return pConn;
			} else {
				pthread_mutex_unlock(&lock); 
				return NULL;
			}
		} else {
			/// 建立的连接数已达到 m_imaxSize 上限
			pthread_mutex_unlock(&lock); 
			return NULL;
		}
	}
}

/**
 * @Synopsis  ReleaseConnection 
 *	回收数据库连接
 * @Param pConn
 */
void CConnctionPool::ReleaseConnection(Connection* pConn)
{
	if (pConn) {
		pthread_mutex_lock(&lock);
		m_connlist.push_back(pConn);
		pthread_mutex_unlock(&lock);
	}
}

/**
 * @Synopsis  TerminateConnectionPool 
 *	销毁连接池，首先要先销毁连接池中的连接
 */
void CConnctionPool::TerminateConnectionPool(void)
{
	list<Connection*>::iterator iconn;
	pthread_mutex_lock(&lock);
	for (iconn=m_connlist.begin(); iconn != m_connlist.end(); ++iconn)
	{
		this->TerminateConnection(*iconn);	/// 销毁连接池中的连接
	}
	m_icurSize = 0;
	m_connlist.clear();		/// 清空连接池中的连接
	pthread_mutex_unlock(&lock);
}

/**
 * @Synopsis  TerminateConnection 
 *	销毁一个连接
 * @Param pConn
 */
void CConnctionPool::TerminateConnection(Connection *pConn)
{
	if (pConn) {
		try{		
			pConn->close();
		} catch(sql::SQLException &e) {
			perror(e.what());
		}
		/*catch(sql::runtime_error  &e) {
			perror(e.what());
		}*/
	}
	delete pConn;
}

/**
 * @Synopsis  GetCurrentSize 
 *	获取当前线程池中的有效的连接数(可选)
 * @Returns   
 */
int CConnctionPool::GetCurrentSize(void)
{
	return m_icurSize;
}







































