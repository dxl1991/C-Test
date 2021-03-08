#pragma once
#include <iostream>
using namespace std;

class Line 
{
public:
	int getLength();
	Line();
	Line(int len); //explicit 防止隐式调用
	~Line();
	Line(const Line &obj);
	friend void printWidth(Line line); //友元函数，不是这个类的函数，但是可以访问该类的私有变量
	friend class BigLine; //友元类，和这个类没关系，但是可以访问这个类的私有变量
	void setWidth(double width); //如果一个函数是内联的，那么在编译时，编译器会把该函数的代码副本放置在每个调用该函数的地方
private:
	int* ptr;
	double width;
};

class BigLine {

public:
	void Print(int width,Line line) {
		line.setWidth(width);
		cout << "width of line1 : " << line.width << endl;
	}
};

void printWidth(Line line);


int getIntNum(); //声明全局函数



