#include<iostream>
using namespace std;

/*
如何用单线程模拟多线程
我们知道在指令的意义下sql实际上是顺次执行的
于是设置每次输入的指令为：事务id + 操作

为了简化，将数据库设置为<int,string>的主键+值的结构
*/

class Database
{
private:
	int max_n;
	string* database;

public:
	Database(int n): max_n(n), database(new string[n]) {}

	bool insert(int key, string val)
	{
		if(key<0 && key>=max_n && database[key]!="")
			return 0;
		database[key]=val;
		return 1;
	}
	string search(int key)
	{
	}
	void print()
	{
		for(int i=0;i<max_n;++i)
			if(database[i] != "")
				cout<<"key: "<<i<<", val: "<<database[i]<<endl;
	}
};

void init(Database& db)
{
	db.insert(0, "aaa");
	db.insert(1, "bbb");
	db.insert(2, "ccc");
	db.insert(3, "ddd");
	db.insert(4, "eee");
	db.insert(5, "fff");
	db.insert(6, "ggg");
	db.insert(7, "hhh");
	db.print();
}
int main()
{
	Database db(10);
	init(db);
	return 0;
}
