#include "TestMap.h"
#include <iostream>
using namespace std;

//map维护一个红黑树，对key进行排序
void TestMap::testMap() 
{
	//insert()
	pair<map<int, string>::iterator,bool> insert_pair1 = personnel.insert(pair<int, string>(123, "123"));//如果key存在了，则插入不进去
	pair<map<int, string>::iterator, bool> insert_pair2 = personnel.insert(map<int, string>::value_type(123, "121"));//和上面一样
	if (insert_pair2.second)
	{
		cout << "insert success ！" << endl;
	}
	else 
	{
		cout << "insert failure ！" << endl;
	}
	
	//数组方式赋值
	personnel[111] = "111";//如果key存在，则覆盖value

	//迭代器遍历
	map<int, string>::iterator iter;
	for (iter = personnel.begin();iter != personnel.end();iter++)
	{
		cout << iter->first << ' ' << iter->second << endl;
	}

	PersonMap uPersonMap;
	uPersonMap[1] = "zero";
	uPersonMap[1] = "one";
	uPersonMap[2] = "two";
	uPersonMap[3] = "three";
	
	//size()
	int size = uPersonMap.size();
	for (int i = 1; i <= size; i++)
	{
		cout << uPersonMap[i] << endl;
	}

	//find()
	iter = uPersonMap.find(5);
	if (iter != uPersonMap.end())
	{
		cout << "Find,the value is " << iter->second << endl;
	}
	else {
		cout << "Do not find" << endl;
	}

	//remove
	int n = uPersonMap.erase(3); //通过key删除，如果删除了返回1，否则返回0
	//uPersonMap.erase(iter);//通过迭代器删除
	//uPersonMap.erase(uPersonMap.begin(), uPersonMap.end());//删除整改map相当于clear()

	map<Studentinfo, int> mapStudent;
	Studentinfo studentinfo;
	studentinfo.niD = 1;
	studentinfo.name = "name_one";
	mapStudent.insert(pair<Studentinfo, int>(studentinfo, 90));
	studentinfo.niD = 2;
	studentinfo.name = "name_two";
	mapStudent.insert(pair<Studentinfo, int>(studentinfo, 80));
	for (map<Studentinfo, int>::iterator studentIter = mapStudent.begin(); studentIter != mapStudent.end(); studentIter++)
		cout << studentIter->first.niD << ' ' << studentIter->first.name << ' ' << studentIter->second << endl;

}
