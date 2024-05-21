#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "database.h"

template<typename Key, typename Val>
class Transaction
{
private:
	int id;
	int iso;
	Database<Key, Val>* db;
public:
	Transaction(Database<Key, Val>& db0, int iso0=1);
	bool start();
	bool commit();
	bool insert(Key key, Val val);
	bool remove(Key key);
	Log<Val>* search(Key key);
	void print(Log<Val>* result);
};


/********************************以下是类的实现*********************************/

template<typename Key, typename Val>
Transaction<Key, Val>::Transaction(Database<Key, Val>& db0, int iso0): db(&db0), id(-1), iso(iso0) {}

template<typename Key, typename Val>
bool Transaction<Key, Val>::start()
{
	// 如果这个事务正在执行，则不能启动
	// id==-1表示事务未在执行
	if(id != -1)
		return 1;
	id = db->start();
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
	cout<<"Transaction "<<id<<" is searching with key = "<<key<<", iso = "<<iso<<endl;
	if(id == -1)
		return nullptr;
	return db->search(key, iso, id);
}

template<typename Key, typename Val>
void Transaction<Key, Val>::print(Log<Val>* result)
{
	if(result == nullptr)
         cout<<"Not Found."<<endl;
     else
         result->print();
}

#endif
