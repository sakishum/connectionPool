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

/// 使用宏替代单例实例
#define ConnPool CConnctionPool::GetInstance()

class CConnctionPool {
private:
	int m_icurSize;		/// 当前已建立的数据库连接	
	int m_imaxSize;		/// 连接池中定义的最大数据库连接数
	string m_suserName;	/// 数据库用户名
	string m_spassword;	/// 数据库用户密码
	string m_surl;		/// 数据库连接 url 

	list<Connection*>m_connlist;	/// 连接池的容器队列
	pthread_mutex_t lock;			/// 线程锁
	Driver *m_pdriver;				/// 连接数据库驱动

	friend class auto_ptr<CConnctionPool>;				/// 使用智能指针
	static auto_ptr<CConnctionPool> auto_ptr_instance;	/// 惟一实例

private:
	CConnctionPool(string url, string userName, string password, int maxSize);	/// 构造函数(注意:构造方法私有)
	~CConnctionPool(void);							/// 析构函数
	Connection* CreateConnection(void);				/// 创建一个连接
	void InitConnection(int iInitialSize);			/// 初始化一个连接
	void TerminateConnection(Connection* pConn);	/// 终止一个数据库连接对象
	void TerminateConnectionPool(void);				/// 终止数据库连接池

public:
	Connection* GetConnection(void);				///	获得数据库连接
	void ReleaseConnection(Connection *pConn);		/// 将数据库连接放回到连接池的容器中
	static CConnctionPool& GetInstance(void);		/// 获取数据库连接池实例(工厂方法)
	int GetCurrentSize(void);						/// 用于观测(非必要)
};

#endif	/*_CONNECTION_POOL_H_*/
