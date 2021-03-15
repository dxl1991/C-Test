#pragma once
class TestClass
{
public:
	TestClass(int a);
	void print();
	virtual void printV();
	//virtual void printV1() = 0; //纯虚函数，没有方法体,该类不能初始化
};


class MyClass : public TestClass 
{
public:
	MyClass(); //会先调用父类的构造函数
	void print();
	void printV();
};