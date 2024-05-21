#ifndef READ_VIEW_H
#define READ_VIEW_H

class ReadView
{
private:
	int max_tra;
	set<int> read_view;
public:
	TraSet();
	int insert();
	bool remove(int tra);
	bool exist(int tra);
	set<int> get_read_view();
};

// class TraSet 维护当前活跃的事务集合
// 构造函数，max_tra表示目前事务id已被使用了几个
TraSet::TraSet(): max_tra(0) {}

// 在当前活跃事务中插入一个新事务
int TraSet::insert()
{
	max_tra++;
	read_view.insert(max_tra);
	return max_tra;
}

// 移除某个已提交事务，成功返回0，失败返回1
bool TraSet::remove(int tra)
{
	if(!exist(tra))
		return 1;
	read_view.erase(tra);
	return 0;
}

// 检查某个事务是否活跃
bool TraSet::exist(int tra)
{
	if(read_view.find(tra) != read_view.end())
		return 1;
	return 0;
}

// 返回ReadView
set<int> TraSet::get_read_view()
{
	return read_view;
}

#endif
