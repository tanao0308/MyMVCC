#ifndef ROW_H
#define ROW_H

#include<iostream>
#include<list>
#include<map>
#include<set>
using namespace std;

template<typename Val>
class Log
{
private:
	Val val;
	int tra;
	bool del;
public:
	Log(Val v, int t, bool d=0);
	Val get_val();
	int get_tra();
	void print();
};

template<typename Val>
class Row
{ // 每一行要存的是 用户数据(val)，事务id(tra)
public:
	list<Log<Val> > row;
	bool empty();
	bool insert(Log<Val> log);
	bool remove(int tra);
	Log<Val>* search(int tra, int iso, set<int>& read_view);
	void print();
};

/************************以下是上面定义的类的实现****************************/

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
Log<Val>* Row<Val>::search(int tra, int iso, set<int>& read_view)
{
	switch(iso)
	{
		case 0: // 读未提交
			if(empty())
				return nullptr;
			else
				return &row.back();
			break;

		// 以下两种方法的区别在ReadView中，读已提交的readview是实时的，而可重复读的readview是事务开始时的
		case 1: // 读已提交
		case 2: // 可重复读
			for(typename list<Log<Val> >::iterator it = row.end(); it != row.begin(); )
			{
				--it;
				// 如果此事务id < 查询的事务的id，且此事务已结束（不在ReadView内），则是答案
				if(it->get_tra() < tra && read_view.find(it->get_tra()) == read_view.end())
					return &(*it);
				// 如果此事务id == 当前事务id，则是答案
				if(it->get_tra() == tra)
					return &(*it);
			}
			return nullptr;
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
}

#endif
