#ifndef DATABASE_H
#define DATABASE_H

#include<iostream>
#include<list>
#include<map>
#include<set>
using namespace std;

class TraSet
{
private:
	int max_tra;
	set<int> active_tra;
public:
	TraSet();
	int insert();
	bool remove(int tra);
	bool exist(int tra);
};

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
	Log<Val>* search(int tra, int iso, TraSet& tra_set);
	void print();
};

template<typename Key, typename Val>
class Database
{
private:
	map<Key, Row<Val> > rows;
	TraSet tra_set;

public:
	Database();

	int start();
	bool commit(int tra);
	bool insert(Key key, Val val, int tra);
	bool remove(Key key, int tra);
	Log<Val>* search(Key key, int tra, int iso);
	void print();
};

template<typename Val>
class Viewer
{
public:
	void print(Log<Val>* result);
};

// C++模板类的定义和实现通常都需要在头文件中
#include "database.cpp"

#endif
