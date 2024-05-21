#include<iostream>
#include<list>
#include<map>
#include<set>
#include "database.h"
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
Log<Val>::Log(Val v, int t, bool d): val(v), tra(t), del(d) {}
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
// 检查当前行是否从未有过记录
template<typename Val>
bool Row<Val>::empty()
{
	if(row.size())
		return 0;
	return 1;
}
// 此行被更新为一条新记录
template<typename Val>
bool Row<Val>::insert(Log<Val> log)
{
	row.push_back(log);
	return 1;
}
// 事务删除此行的记录，等价于此行插入一条被标记为删除的空记录
template<typename Val>
bool Row<Val>::remove(int tra)
{
	if(empty())
		return 0;
	Log<Val> log(row.back().val, tra, 1);
	row.push_back();
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
				if(it->get_tra() < tra && !tra_set.exist(it->get_tra()))
					return &(*it);
				if(it->get_tra() == tra)
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
// 某个事物删除一条记录
template<typename Key, typename Val>
bool Database<Key, Val>::remove(Key key, int tra)
{
	if(!tra_set.exist(tra))
		return nullptr;
	if(rows.count(key) == 0)
		return nullptr;
	return rows[key].remove(tra);
}
// 某个事务查找一条记录
template<typename Key, typename Val>
Log<Val>* Database<Key, Val>::search(Key key, int tra, int iso)
{
	cout<<"Transaction "<<tra<<" is searching with key = "<<key<<", iso = "<<iso<<endl;
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

