#pragma once
#include <iostream>
using namespace std;

class Line 
{
public:
	int getLength();
	Line();
	Line(int len); //explicit ��ֹ��ʽ����
	~Line();
	Line(const Line &obj);
	friend void printWidth(Line line); //��Ԫ���������������ĺ��������ǿ��Է��ʸ����˽�б���
	friend class BigLine; //��Ԫ�࣬�������û��ϵ�����ǿ��Է���������˽�б���
	void setWidth(double width); //���һ�������������ģ���ô�ڱ���ʱ����������Ѹú����Ĵ��븱��������ÿ�����øú����ĵط�
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


int getIntNum(); //����ȫ�ֺ���



