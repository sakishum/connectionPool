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
	Connection* pconn = NULL;
	CConnctionPool::GetInstance();
	for (int i = 0; i < 100; ++i)
	{
		cout<<i<<endl;
		pconn = CConnctionPool::GetInstance().GetConnection();
		if (pconn != NULL)
		{
			sql::Statement  *pstmt; 
			sql::ResultSet  *pres; 
			cout<<"Successful! Current Size is "<<CConnctionPool::GetInstance().GetCurrentSize()<<endl;
			pstmt = pconn->createStatement();
			pstmt->execute("use cusemysql");
			pres = pstmt->executeQuery("select * from children");
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
		}
	}
	cout<<"--Done.--"<<endl;
	return 0;
}
