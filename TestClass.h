#pragma once
class TestClass
{
public:
	TestClass(int a);
	void print();
	virtual void printV();
	//virtual void printV1() = 0; //���麯����û�з�����,���಻�ܳ�ʼ��
};


class MyClass : public TestClass 
{
public:
	MyClass(); //���ȵ��ø���Ĺ��캯��
	void print();
	void printV();
};