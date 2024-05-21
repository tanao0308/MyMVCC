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

// class TraSet 维护当前活跃的事务集合
// 构造函数，max_tra表示目前事务id已被使用了几个
TraSet::TraSet(): max_tra(0) {}
// 在当前活跃事务中插入一个新事务
int TraSet::insert()
{
	max_tra++;
	active_tra.insert(max_tra);
	return max_tra;
}
// 移除某个已提交事务，成功返回0，失败返回1
bool TraSet::remove(int tra)
{
	if(!exist(tra))
		return 1;
	active_tra.erase(tra);
	return 0;
}
// 检查某个事务是否活跃
bool TraSet::exist(int tra)
{
	if(active_tra.find(tra) != active_tra.end())
		return 1;
	return 0;
}

// class Log 单条数据的类
template<typename Val>
Log<Val>::Log(Val v, int t): val(v), tra(t) {}
// 得到记录的val值
template<typename Val>
Val Log<Val>::get_val()
{
	return val;
}
// 得到产生这条记录的事务
template<typename Val>
int Log<Val>::get_tra()
{
	return tra;
}
// 输出记录
template<typename Val>
void Log<Val>::print()
{
	cout<<"Val = "<<val<<", Tra = "<<tra<<endl;
}


// class Row 一个主键对应的行的类
// 此行被更新为一条新记录
template<typename Val>
bool Row<Val>::insert(Log<Val> log)
{
	row.push_back(log);
	return 1;
}
// 事务在此行的版本中找到符合隔离级别的某条记录
template<typename Val>
Log<Val>* Row<Val>::search(int tra, int iso, TraSet& tra_set)
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
					return &(*it);
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
// 打印一行的所有版本（从老到新）
template<typename Val>
void Row<Val>::print()
{
	for(typename list<Log<Val> >::iterator it = row.begin(); it != row.end(); ++it)
		it->print();
	// row.back().print();
}
// 检查当前行是否从未有过记录
template<typename Val>
bool Row<Val>::empty()
{
	if(row.size())
		return 0;
	return 1;
}

// class Database 数据库类
template<typename Key, typename Val>
Database<Key, Val>::Database() {}
// 向数据库申请一个事务
template<typename Key, typename Val>
int Database<Key, Val>::start()
{
	int tra = tra_set.insert();
	return tra;
}
// 提交一个事务
template<typename Key, typename Val>
bool Database<Key, Val>::commit(int tra)
{
	return tra_set.remove(tra);
}
// 某个事务插入一条记录
template<typename Key, typename Val>
bool Database<Key, Val>::insert(Key key, Val val, int tra)
{
	if(!tra_set.exist(tra))
		return 1; // 失败返回0
	Log<Val> log(val, tra);
	return rows[key].insert(log);
}
// 某个事务查找一条记录
template<typename Key, typename Val>
Log<Val>* Database<Key, Val>::search(Key key, int tra, int iso)
{
	if(!tra_set.exist(tra))
		return nullptr;
	if(rows.count(key) == 0)
		return nullptr;
	return rows[key].search(tra, iso, tra_set);
}
// 数据库打印全部最新内容
template<typename Key, typename Val>
void Database<Key, Val>::print()
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

// class Viewer 可视化函数类
// 输出当前Log内容
template<typename Val>
void Viewer<Val>::print(Log<Val>* result)
{
	if(result == nullptr)
		cout<<"Not Found."<<endl;
	else
		result->print();
}


template<typename Key, typename Val>
void init(Database<Key, Val>& db)
{
	cout<<"-------Initializing Database-------"<<endl;
	int tra = db.start();
	db.insert(0, "aa", tra);
	db.insert(1, "bb", tra);
	db.insert(2, "cc", tra);
	db.insert(3, "dd", tra);
	db.insert(4, "ee", tra);
	db.insert(5, "ff", tra);
	db.insert(6, "gg", tra);
	db.insert(7, "hh", tra);
	db.insert(8, "ii", tra);
	if(db.commit(tra))
		exit(1);
	db.print();
	cout<<"-------------Finished--------------"<<endl;
}


int main()
{
	Database<int, string> db;
	init<int, string>(db);

	Viewer<string> vie;

	int tar = db.start();	
	Log<string>* result = db.search(0, tar, 0);
	vie.print(result);

	result = db.search(3, tar, 1);
	vie.print(result);

	result = db.search(10, tar, 0);
	vie.print(result);

	return 0;
}
