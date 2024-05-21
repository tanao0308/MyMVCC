#include<iostream>
#include "transaction.h"

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
	Database<int, string>& db = init<int, string>();

	Transaction<int, string> tra1(db, 1);
	Transaction<int, string> tra2(db, 1);
	
	tra1.start();
	tra1.insert(9, "abc");

	tra2.start();
	tra2.print(tra2.search(9));

	tra1.commit();

	tra2.print(tra2.search(9));

	delete &db;
}


int main()
{
	test_read_uncommitted();
	return 0;
}
