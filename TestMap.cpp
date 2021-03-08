#include "TestMap.h"
#include <iostream>
using namespace std;

//mapά��һ�����������key��������
void TestMap::testMap() 
{
	//insert()
	pair<map<int, string>::iterator,bool> insert_pair1 = personnel.insert(pair<int, string>(123, "123"));//���key�����ˣ�����벻��ȥ
	pair<map<int, string>::iterator, bool> insert_pair2 = personnel.insert(map<int, string>::value_type(123, "121"));//������һ��
	if (insert_pair2.second)
	{
		cout << "insert success ��" << endl;
	}
	else 
	{
		cout << "insert failure ��" << endl;
	}
	
	//���鷽ʽ��ֵ
	personnel[111] = "111";//���key���ڣ��򸲸�value

	//����������
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
	int n = uPersonMap.erase(3); //ͨ��keyɾ�������ɾ���˷���1�����򷵻�0
	//uPersonMap.erase(iter);//ͨ��������ɾ��
	//uPersonMap.erase(uPersonMap.begin(), uPersonMap.end());//ɾ������map�൱��clear()

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
