/**
 * @file main.cpp
 * @Synopsis  Test MySQL connection pool with pThread
 * @author Saki Shum, sakishum1118@gmail.com
 * @version 0.0.1
 * @date 2013-03-05
 */
#include <iostream>
#include "connect_pool.h"

using namespace std;

int main(int argc, char* argv[])
{
	CConnctionPool::GetInstance();
/*	for (int i = 0; i < 8000; ++i)*/
	int i = 0;
	{
		cout<<i<<endl;
		Connection* pconn = NULL;
		pconn = CConnctionPool::GetInstance().GetConnection();
		if (pconn != NULL)
		{
			char buffer[100];
			sql::Statement  *pstmt; 
			sql::ResultSet  *pres; 
			cout<<"Successful! Current Size is "<<CConnctionPool::GetInstance().GetCurrentSize()<<endl;
			pstmt = pconn->createStatement();
			pstmt->execute("use cusemysql");
			//pres = pstmt->executeQuery("select * from children");
			//sprintf(buffer, "update children set fname='更改%d' where childno=%d;", i, i);
			//pstmt->execute(buffer);
			
			/// 执行存储过程
			pres = pstmt->executeQuery("call test_procedure()");		
			int nRows = pres->rowsCount();  
			cout<<nRows<<endl;
			
			while (pres->next())
			{
				int id = pres->getInt("childno");
				cout <<"id:"<< id << endl;
				string name = pres->getString("fname");
				cout << name <<"  "<< pres->getString(3)<<endl;
			}
			delete pres;
			delete pstmt;
			CConnctionPool::GetInstance().ReleaseConnection(pconn);
			//delete pconn;
			//pconn = NULL;
		}
	}
	cout<<"--Done.--"<<endl;
	return 0;
}
