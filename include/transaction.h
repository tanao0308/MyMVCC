#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "database.h"

template<typename Key, typename Val>
class Transaction
{
private:
	int id;
	int iso;
	set<int> read_view;
	Database<Key, Val>* db;
public:
	Transaction(Database<Key, Val>& db0);
	bool start(int iso0=2);
	bool commit();
	bool insert(Key key, Val val);
	bool remove(Key key);
	Log<Val>* search(Key key);
	Log<Val>* search(Key key, bool print);
};


/********************************以下是类的实现*********************************/

// 事务用数据库引用初始化，设置隔离级别为repeatable read
template<typename Key, typename Val>
Transaction<Key, Val>::Transaction(Database<Key, Val>& db0): db(&db0), id(-1), iso(2) {}

template<typename Key, typename Val>
bool Transaction<Key, Val>::start(int iso0)
{
	// 如果这个事务正在执行，则不能启动
	// id==-1表示事务未在执行
	if(id != -1)
		return 1;
	id = db->start();
	iso = iso0;
	if(iso == 1)
		read_view.clear();
	else if(iso == 2)
		read_view = db->get_read_view();
	return 0;
}

template<typename Key, typename Val>
bool Transaction<Key, Val>::commit()
{
	if(id == -1)
		return 1;
	bool result = db->commit(id);
	id = -1;
	return result;
}

template<typename Key, typename Val>
bool Transaction<Key, Val>::insert(Key key, Val val)
{
	if(id == -1)
		return 1;
	return db->insert(key, val, id);
}

template<typename Key, typename Val>
bool Transaction<Key, Val>::remove(Key key)
{
	if(id == -1)
		return 1;
	return db->remove(key);
}

template<typename Key, typename Val>
Log<Val>* Transaction<Key, Val>::search(Key key)
{
	if(iso == 1)
		read_view = db->get_read_view();
	if(id == -1)
		return nullptr;
	return db->search(key, iso, id);
}

template<typename Key, typename Val>
Log<Val>* Transaction<Key, Val>::search(Key key, bool print)
{
	cout<<"Transaction "<<id<<" is searching with key = "<<key<<", iso = "<<iso<<endl;
	
	if(iso == 1) // 如果隔离级别为read committed，则获取实时的ReadView
		read_view = db->get_read_view();
	if(id == -1)
		return nullptr;
	Log<Val>*result = db->search(key, iso, id, read_view);
	if(result == nullptr)
        cout<<"Not Found."<<endl;
    else
        result->print();
	return result;
}

#endif
