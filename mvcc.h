#ifndef MVCC_H
#define MVCC_H

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
public:
	Log(Val v, int t): val(v), tra(t) {}
	Val get_val();
	int get_tra();
	void print();
};

template<typename Val>
class Row
{ // 每一行要存的是 用户数据(val)，事务id(tra)
public:
	list<Log<Val> > row;
	bool insert(Log<Val> log);
	Log<Val>* search(int tra, int iso, TraSet& tra_set);
	void print();
	bool empty();
};

class TraSet
{
private:
	int max_tra;
	set<int> active_tra;
public:
	TraSet(): max_tra(0) {}
	void insert_tra(int tra);
	bool remove_tra(int tra);
	bool exist_tra(int tra);
};

template<typename Key, typename Val>
class Database
{
private:
	map<Key, Row<Val> > rows;
	TraSet tra_set;

public:
	Database(): max_tra(0) {}

	int start();
	bool commit(int tra);
	bool insert(Key key, Val val, int tra);
	Log<Val>* search(Key key, int tra, int iso);
	void print();
};

template<typename Val>
class Viewer
{
public:
	void print(Log<Val>* result);
};


#endif
