/**
 * @file connect_pool.cpp
 * @Synopsis  Connection Pool for MySQL with pThread.
 * @author Saki Shum, sakishum1118@gmail.com
 * @version 0.0.1
 * @date 2013-03-05
 */
#include <stdexcept> 
#include <exception>  
#include <stdio.h>  
#include "connect_pool.h"

using namespace std;
using namespace sql;

const string g_sdefaultHost = "localhost";
const string g_sdefaultUser = "root";
const string g_sdefaultPass = "qwer1234";
const int g_imaxSize = 50;

auto_ptr<CConnctionPool> CConnctionPool::auto_ptr_instance;

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

CConnctionPool::~CConnctionPool(void)
{
	this->TerminateConnectionPool();
}

CConnctionPool& CConnctionPool::GetInstance(void)
{
	if (auto_ptr_instance.get() == 0) {
		auto_ptr_instance.reset(new CConnctionPool(g_sdefaultHost, g_sdefaultUser, g_sdefaultPass, g_imaxSize));
	}
	return *(auto_ptr_instance.get());
}

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

Connection* CConnctionPool::GetConnection(void)
{
	Connection *pConn;
	pthread_mutex_lock(&lock);
	if (m_connlist.size() > 0) {
		pConn = m_connlist.front();
		m_connlist.pop_front();
		if (pConn->isClosed()) {
			delete pConn;
			pConn = this->CreateConnection();
		}
		if (NULL == pConn) {
			--m_icurSize;
		}
		pthread_mutex_unlock(&lock);
		return pConn;
	} else {
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
			pthread_mutex_unlock(&lock); 
			return NULL;
		}
	}
}

void CConnctionPool::ReleaseConnection(Connection* pConn)
{
	if (pConn) {
		pthread_mutex_lock(&lock);
		m_connlist.push_back(pConn);
		pthread_mutex_unlock(&lock);
	}
}

void CConnctionPool::TerminateConnectionPool(void)
{
	list<Connection*>::iterator iconn;
	pthread_mutex_lock(&lock);
	for (iconn=m_connlist.begin(); iconn != m_connlist.end(); ++iconn)
	{
		this->TerminateConnection(*iconn);
	}
	m_icurSize = 0;
	m_connlist.clear();
	pthread_mutex_unlock(&lock);
}

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

int CConnctionPool::GetCurrentSize(void)
{
	return m_icurSize;
}







































