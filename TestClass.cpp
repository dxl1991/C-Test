#include <iostream>
#include "TestClass.h"

using namespace std;

TestClass::TestClass(int a)
{
	cout << "TestClass()," <<a<< endl;
}

void TestClass::print()
{
	cout << "this is TestClass.print()" << endl;
}

void TestClass::printV()
{
	cout << "this is TestClass.printV()" << endl;
}


MyClass::MyClass():TestClass(3)
{
	cout << "MyClass()" << endl;
}

void MyClass::print()
{
	cout << "this is MyClass.print()" << endl;
}

void MyClass::printV()
{
	cout << "this is MyClass.printV()" << endl;
}