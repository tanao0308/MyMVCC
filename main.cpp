#include<iostream>
#include<list>
#include<map>
using namespace std;

/*
如何用单线程模拟多线程
我们知道在指令的意义下sql实际上是顺次执行的
于是设置每次输入的指令为：事务id + 操作

为了简化，将数据库设置为<int,string>的主键+值的结构
*/

template<typename Val>
class Log
{
private:
	Val val;
	int tra;
public:
	Log(Val v, int t): val(v), tra(t) {}
	void print()
	{
		cout<<"Val = "<<val<<", Tra = "<<tra<<endl;
	}
};

template<typename Val>
class Row
{ // 每一行要存的是 用户数据(val)，事务id(tra)
public:
	list<Log<Val> > row;
	bool insert(Log<Val> log)
	{
		row.push_back(log);
		return 1;
	}
	Log<Val>* search(int tra, int iso)
	{
		switch(iso)
		{
			case 0: // 读未提交
				if(empty())
					return nullptr;
				else
					return &row.back();
				break;
			case 1: // 读已提交
				break;
			case 2: // 可重复读
				break;
			case 3: // 串行化
				break;
			default:
				break;
		}
		return nullptr;
	}
	void print()
	{
		for(typename list<Log<Val> >::iterator it = row.begin(); it != row.end(); ++it)
			it->print();
		// row.back().print();
	}
	bool empty()
	{
		if(row.size())
			return 0;
		return 1;
	}
};

template<typename Key, typename Val>
class Database
{
private:
	map<Key, Row<Val> > rows;

public:
	Database() {}

	bool insert(Key key, Val val, int tra)
	{
		Log<Val> log(val, tra);
		return rows[key].insert(log);
	}
	Log<Val>* search(Key key, int tra, int iso)
	{
		if(rows.count(key) == 0)
			return nullptr;
		return rows[key].search(tra, iso);
	}

};

class Viewer
{
public:
	
	void print()
	{
		for(typename map<key, row<val> >::iterator it = rows.begin(); it != rows.end(); ++it)
		{
			key cur_key = it->first;
			row<val> cur_row = it->second;
			if(!cur_row.empty())
			{
				cout<<"key = "<<cur_key<<", ";
				cur_row.print();
			}
		}
	}
};

template<typename Key, typename Val>
void init(Database<Key, Val>& db)
{
	db.insert(0, "aa", 0);
	db.insert(1, "bb", 0);
	db.insert(2, "cc", 0);
	db.insert(3, "dd", 0);
	db.insert(4, "ee", 0);
	db.insert(5, "ff", 0);
	db.insert(6, "gg", 0);
	db.insert(7, "hh", 0);
	db.insert(8, "ii", 0);
	cout<<"--------------"<<endl;
	db.print();
	cout<<"--------------"<<endl;
}


int main()
{
	Database<int, string> db;
	init<int, string>(db);
	Log<string>* result = db.search(0, 1, 0);
	if(result == nullptr)
		cout<<"Not Found."<<endl;
	else
		result->print();

	result = db.search(3, 1, 0);
	if(result == nullptr)
		cout<<"Not Found."<<endl;
	else
		result->print();



	return 0;
}
