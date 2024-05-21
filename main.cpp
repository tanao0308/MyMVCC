#include<iostream>
#include "mvcc.h"

template<typename Key, typename Val>
void init(Database<Key, Val>& db)
{
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
}


int main()
{
    Database<int, string> db;
    init<int, string>(db);

    Viewer<string> vie;

    int tar = db.start();
    Log<string>* result = db.search(0, tar, 0);
    vie.print(result);

    result = db.search(3, tar, 1);
    vie.print(result);

    result = db.search(10, tar, 0);
    vie.print(result);

    return 0;
}
