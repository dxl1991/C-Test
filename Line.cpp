#include <iostream>
#include "Line.h"

using namespace std;

Line::Line(int len) 
{
	cout << "���ù��캯��" << endl;
	ptr = new int;
	*ptr = len;
}

Line::Line(const Line &obj)
{
	cout << "���ÿ������캯��" << endl;
	ptr = new int;
	*ptr = *obj.ptr;
}

Line::~Line() 
{
	cout << "�ͷ��ڴ�" << endl;
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
	cout << "����ȫ�ֺ���" << endl;
}
void Line::setWidth(double width) {
	this->width = width;
	printTest(); //������Ҫ�ڶ������
}

//ȫ�ֺ���ֻ�ܶ�����cpp�ļ��У�
void displayLine(Line line) {
	cout << "line ��С : " << line.getLength() << endl;
}

void printWidth(Line line) {
	cout << "width of line2 :" << line.width << endl;
}

int getIntNum()
{
	return 1314;
}

