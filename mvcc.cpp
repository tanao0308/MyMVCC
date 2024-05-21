#include<iostream>
#include<list>
#include<map>
#include<set>
#include "mvcc.h"
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
	Val get_val()
	{
		return val;
	}
	int get_tra()
	{
		return tra;
	}
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
	Log<Val>* search(int tra, int iso, TraSet& tra_set)
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

				for(typename list<Log<Val> >::iterator it = row.end(); it != row.begin(); )
				{
					--it;
					if(it->get_tra() < tra && tra_set.exist(it->get_tra()))
						return it;
				}
				return nullptr;

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

class TraSet
{
private:
	int max_tra;
	set<int> active_tra;
public:
	TraSet(): max_tra(0) {}
	void insert_tra(int tra)
	{
		active_tra.insert(tra);
	}
	bool remove_tra(int tra)
	{
		if(!exist_tra(tra))
			return 1;
		active_tra.erase(tra);
		return 0;
	}
	bool exist_tra(int tra)
	{
		if(active_tra.find(tra) != active_tra.end())
			return 1;
		return 0;
	}
};

template<typename Key, typename Val>
class Database
{
private:
	map<Key, Row<Val> > rows;
	TraSet tra_set;

public:
	Database(): max_tra(0) {}

	int start()
	{
		tra_set.insert(++max_tra);
		return max_tra;
	}
	bool commit(int tra)
	{
		if(tra_set.remove(tra))
			return 0;
		return 1;
	}
	bool insert(Key key, Val val, int tra)
	{
		if(!tra_set.exist(tra))
			return 1; // 失败返回0
		Log<Val> log(val, tra);
		return rows[key].insert(log);
	}
	Log<Val>* search(Key key, int tra, int iso)
	{
		if(!exist_tra(tra))
			return nullptr;
		if(rows.count(key) == 0)
			return nullptr;
		return rows[key].search(tra, iso, tra_set);
	}
	void print()
	{
		for(typename map<Key, Row<Val> >::iterator it = rows.begin(); it != rows.end(); ++it)
		{
			Key cur_key = it->first;
			Row<Val> cur_row = it->second;
			if(!cur_row.empty())
			{
				cout<<"key = "<<cur_key<<", ";
				cur_row.print();
			}
		}
	}
};

template<typename Val>
class Viewer
{
public:
	void print(Log<Val>* result)
	{
		if(result == nullptr)
			cout<<"Not Found."<<endl;
		else
			result->print();
	}
};

template<typename Key, typename Val>
void init(Database<Key, Val>& db)
{
	cout<<"-------Initializing Database-------"<<endl;
	db.insert(0, "aa", 0);
	db.insert(1, "bb", 0);
	db.insert(2, "cc", 0);
	db.insert(3, "dd", 0);
	db.insert(4, "ee", 0);
	db.insert(5, "ff", 0);
	db.insert(6, "gg", 0);
	db.insert(7, "hh", 0);
	db.insert(8, "ii", 0);
	db.print();
	cout<<"-------------Finished--------------"<<endl;
}


int main()
{
	Database<int, string> db;
	init<int, string>(db);

	Viewer<string> vie;

	Log<string>* result = db.search(0, 1, 0);
	vie.print(result);

	result = db.search(3, 1, 0);
	vie.print(result);

	result = db.search(10, 1, 0);
	vie.print(result);

	return 0;
}
