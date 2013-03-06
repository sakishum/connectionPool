/**
 * @file connect_pool.h
 * @Synopsis  Connection Pool for MySQL with pThread.
 * @author Saki Shum, sakishum1118@gmail.com
 * @version 0.0.1
 * @date 2013-03-05
 */
/// Modify by Saki Shum @ 03/06/2013

#ifndef _CONNECTION_POOL_H_
#define _CONNECTION_POOL_H_

#include <iostream>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <pThread.h>
#include <list>

using namespace std;
using namespace sql;

class CConnctionPool {
private:
	int m_icurSize;	
	int m_imaxSize;
	string m_suserName;
	string m_spassword;
	string m_surl;

	list<Connection*>m_connlist;
	pthread_mutex_t lock;
	Driver *m_pdriver;

	friend class auto_ptr<CConnctionPool>;
	static auto_ptr<CConnctionPool> auto_ptr_instance;

private:
	CConnctionPool(string url, string userName, string password, int maxSize);
	~CConnctionPool(void);
	Connection* CreateConnection(void);
	void InitConnection(int iInitialSize);
	void TerminateConnection(Connection* pConn);
	void TerminateConnectionPool(void);

public:
	Connection* GetConnection(void);
	void ReleaseConnection(Connection *pConn);
	static CConnctionPool& GetInstance(void);
	int GetCurrentSize(void);
};

#endif	/*_CONNECTION_POOL_H_*/
