#include <iostream>
#include "Line.h"

using namespace std;

Line::Line(int len) 
{
	cout << "调用构造函数" << endl;
	ptr = new int;
	*ptr = len;
}

Line::Line(const Line &obj)
{
	cout << "调用拷贝构造函数" << endl;
	ptr = new int;
	*ptr = *obj.ptr;
}

Line::~Line() 
{
	cout << "释放内存" << endl;
	delete ptr;
}

int Line::getLength() {
	return *ptr;
}

Line::Line()
{
}

void printTest()
{
	cout << "调用全局函数" << endl;
}
void Line::setWidth(double width) {
	this->width = width;
	printTest(); //调用需要在定义后面
}

//全局函数只能定义在cpp文件中？
void displayLine(Line line) {
	cout << "line 大小 : " << line.getLength() << endl;
}

void printWidth(Line line) {
	cout << "width of line2 :" << line.width << endl;
}

int getIntNum()
{
	return 1314;
}

