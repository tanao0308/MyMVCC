#ifndef DATABASE_H
#define DATABASE_H

#include "row.h"
#include "read_view.h"
using namespace std;

template<typename Key, typename Val>
class Database
{
private:
	map<Key, Row<Val> > rows;
	ReadView viewer;

public:
	Database();

	int start();
	bool commit(int tra);
	bool insert(Key key, Val val, int tra);
	bool remove(Key key, int tra);
	Log<Val>* search(Key key, int iso, int tra, set<int>& read_view);
	set<int> get_read_view();
	void print();
};

/*********************************以下是上述类的实现***********************************/

// class Database 数据库类
template<typename Key, typename Val>
Database<Key, Val>::Database() {}

// 向数据库申请一个事务
template<typename Key, typename Val>
int Database<Key, Val>::start()
{
	int tra = viewer.insert();
	return tra;
}

// 提交一个事务
template<typename Key, typename Val>
bool Database<Key, Val>::commit(int tra)
{
	return viewer.remove(tra);
}

// 某个事务插入一条记录
template<typename Key, typename Val>
bool Database<Key, Val>::insert(Key key, Val val, int tra)
{
	if(!viewer.exist(tra))
		return 1; // 失败返回0
	Log<Val> log(val, tra);
	return rows[key].insert(log);
}

// 某个事物删除一条记录
template<typename Key, typename Val>
bool Database<Key, Val>::remove(Key key, int tra)
{
	if(!viewer.exist(tra))
		return nullptr;
	if(rows.count(key) == 0)
		return nullptr;
	return rows[key].remove(tra);
}

// 某个事务查找一条记录
template<typename Key, typename Val>
Log<Val>* Database<Key, Val>::search(Key key, int iso, int tra, set<int>& read_view)
{
	if(!viewer.exist(tra))
		return nullptr;
	return rows[key].search(tra, iso, read_view);
}


// 返回ReadView
template<typename Key, typename Val>
set<int> Database<Key, Val>::get_read_view()
{
	return viewer.get_read_view();
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

#endif
