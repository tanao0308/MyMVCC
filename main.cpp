#include<iostream>
#include "database.h"

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
	int iso = 1;
	Database<int, string>& db = init<int, string>();
    Viewer<string> vie;

    int tra1, tra2;
	
	tra1 = db.start();
	db.insert(9, "abc", tra1);
    vie.print(db.search(9, tra1, iso));

	tra2 = db.start();
	vie.print(db.search(9, tra2, iso));

	db.commit(tra1);

	vie.print(db.search(9, tra2, iso));

	delete &db;
}


int main()
{
	test_read_uncommitted();
	return 0;
}
