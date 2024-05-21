#include<iostream>
#include "mvcc.h"

template<typename Key, typename Val>
Database<Key, Val>& init()
{
	Database<Key, Val>& db = *(new Database<Key, Val>());
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
	return db;
}

void test_read_uncommitted()
{
	int iso = 0;
	Database<int, string>& db = init<int, string>();
    Viewer<string> vie;

    int tar1, tar2;
    Log<string>* result;
	
	tar1 = db.start();
	result = db.search(0, tar1, iso);
    vie.print(result);

    result = db.search(3, tar1, iso);
    vie.print(result);

    result = db.search(10, tar1, iso);
    vie.print(result);

	delete &db;
}


int main()
{
	test_read_uncommitted();
	return 0;
}
