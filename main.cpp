/**
 * @file main.cpp
 * @Synopsis  Test MySQL connection pool with pThread
 * @author Saki Shum, sakishum1118@gmail.com
 * @version 0.1.7
 * @date 2013-03-05
 */

#include <iostream>
#include <time.h>
#include "connect_pool.h"

using namespace std;
using namespace sql;

int main(int argc, char* argv[])
{
	clock_t tStart, tStop;
	tStart = clock();

	//CConnctionPool::GetInstance();	/// 非必要
	for (int i = 0; i < 8000; ++i)
	//int i = 0;
	{
		cout<<i<<endl;
		Connection* pconn = MySQLPool.GetConnection();	/// 获取连接
		if (pconn != NULL)
		{
			char buffer[100];
			auto_ptr<Statement> pstmt(pconn->createStatement()); 
			cout<<"Successful! Current Size is "<<CConnctionPool::GetInstance().GetCurrentSize()<<endl;
			pstmt->execute("use cusemysql");
			//pres = pstmt->executeQuery("select * from children");
			//sprintf(buffer, "update children set fname='更改%d' where childno=%d;", i, i);
			//pstmt->execute(buffer);
			
			/// 执行存储过程
			auto_ptr<ResultSet> pres(pstmt->executeQuery("call test_procedure()"));		
			int nRows = pres->rowsCount();  
			cout<<nRows<<endl;
			
			while (pres->next())
			{
				int id = pres->getInt("childno");
				cout <<"id:"<< id << endl;
				string name = pres->getString("fname");
				cout << name <<"  "<< pres->getString(3)<<endl;
			}
			MySQLPool.ReleaseConnection(pconn);	/// 释放连接
		}
	}
	tStop = clock();
	printf("C++: %ldms.\n",(tStop - tStart) * 1000 / CLOCKS_PER_SEC);
	cout<<"--Done.--"<<endl;
	return 0;
}
