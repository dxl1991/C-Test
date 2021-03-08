﻿// C-Test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "Line.h"
#include "TestMap.h"
#include "TestTemplate.h"
#include "TestThread.h"
#include "Game.h"
#define  abc 1024
using namespace std;

static int objectCount;

void testLine()
{
	//extern void printWidth(Line); 
	extern void displayLine(Line); //使用其他cpp文件中的全局函数，以为这个函数没有在.h文件中声明
	objectCount = 122;
	Line line(10);
	//Line line = 10; //隐试调用构造函数
	BigLine big;
	line.setWidth(10);
	printWidth(line);
	displayLine(line);
	big.Print(20, line);
	cout << getIntNum() << endl;
	cout << abc << endl;
	int t1 = 1111;
	int t2 = 2222;
	//swap1(t1, t2);
	//Stack<int> intStack;
	//intStack.push(1);
	//intStack.push(2);
	//intStack.push(3);
	//while (!intStack.isEmpty()) {
	//	printf("num:%d\n", intStack.pop());
	//}
	cout << "t1="<<t1<<",t2=" << t2 << endl;
	getchar();
}

void example1()
{
	int *p = new int(3); //new出来的是一段空间的首地址

	cout << p << endl;  //p是一个地址

	int q = *new int; //new开辟的空间在堆上，而一般声明的变量存放在栈上
	q = 1;

	cout << q << endl;//q是一个值

	int* a = new int[3]; //a是一个数组

	for (int i=0;i<3;i++)
	{
		a[i] = i;
	}

}

void testString() 
{
	char site[] = "RUNOOB"; //c风格字符串,一个char数组，默认以\0结尾
	cout << site << endl;
	char str1[13] = "hello";
	char str2[] = "world";
	char str3[6]; 
	strcpy_s(str3, str1);//注意str3要多一个长度
	cout << str3 << endl;
	strcat_s(str1, str2);
	cout << str1 << endl;
	cout << strlen(str1) << endl;  //输出10

	string a = "aaaaaa";
	string b = "bbbbbb";
	string c;
	c = a;
	cout << c << endl;
	a += b;
	cout << a << endl;
	cout << a.size() << endl;
	getchar();
}


int main()
{
	//testLine();
	//TestMap testMap;
	//testMap.testMap();
	//example1();
	//testString();
	//TestThread testThread;
	//testThread.test1();
	int a, b;
	Game game;
	while (true)
	{
		cout << endl;
		cout << "请输入第一个坐标:" << endl;
		cin >> a;
		cout << "请输入第二个坐标:" << endl;
		cin >> b;
		game.swapSprite(a, b);
	}

	getchar();
}

void printData() {
	cout << "Hello World!\n";
	cout << "type: \t\t" << "************size**************" << endl;
	cout << "bool: \t\t" << "所占字节数：" << sizeof(bool);
	cout << "\t最大值：" << (numeric_limits<bool>::max)();
	cout << "\t\t最小值：" << (numeric_limits<bool>::min)() << endl;
	cout << "char: \t\t" << "所占字节数：" << sizeof(char);
	cout << "\t最大值：" << (numeric_limits<char>::max)();
	cout << "\t\t最小值：" << (numeric_limits<char>::min)() << endl;
	cout << "signed char: \t" << "所占字节数：" << sizeof(signed char);
	cout << "\t最大值：" << (numeric_limits<signed char>::max)();
	cout << "\t\t最小值：" << (numeric_limits<signed char>::min)() << endl;
	cout << "unsigned char: \t" << "所占字节数：" << sizeof(unsigned char);
	cout << "\t最大值：" << (numeric_limits<unsigned char>::max)();
	cout << "\t\t最小值：" << (numeric_limits<unsigned char>::min)() << endl;
	cout << "wchar_t: \t" << "所占字节数：" << sizeof(wchar_t);
	cout << "\t最大值：" << (numeric_limits<wchar_t>::max)();
	cout << "\t\t最小值：" << (numeric_limits<wchar_t>::min)() << endl;
	cout << "short: \t\t" << "所占字节数：" << sizeof(short);
	cout << "\t最大值：" << (numeric_limits<short>::max)();
	cout << "\t\t最小值：" << (numeric_limits<short>::min)() << endl;
	cout << "int: \t\t" << "所占字节数：" << sizeof(int);
	cout << "\t最大值：" << (numeric_limits<int>::max)();
	cout << "\t最小值：" << (numeric_limits<int>::min)() << endl;
	cout << "unsigned: \t" << "所占字节数：" << sizeof(unsigned);
	cout << "\t最大值：" << (numeric_limits<unsigned>::max)();
	cout << "\t最小值：" << (numeric_limits<unsigned>::min)() << endl;
	cout << "long: \t\t" << "所占字节数：" << sizeof(long);
	cout << "\t最大值：" << (numeric_limits<long>::max)();
	cout << "\t最小值：" << (numeric_limits<long>::min)() << endl;
	cout << "unsigned long: \t" << "所占字节数：" << sizeof(unsigned long);
	cout << "\t最大值：" << (numeric_limits<unsigned long>::max)();
	cout << "\t最小值：" << (numeric_limits<unsigned long>::min)() << endl;
	cout << "double: \t" << "所占字节数：" << sizeof(double);
	cout << "\t最大值：" << (numeric_limits<double>::max)();
	cout << "\t最小值：" << (numeric_limits<double>::min)() << endl;
	cout << "long double: \t" << "所占字节数：" << sizeof(long double);
	cout << "\t最大值：" << (numeric_limits<long double>::max)();
	cout << "\t最小值：" << (numeric_limits<long double>::min)() << endl;
	cout << "float: \t\t" << "所占字节数：" << sizeof(float);
	cout << "\t最大值：" << (numeric_limits<float>::max)();
	cout << "\t最小值：" << (numeric_limits<float>::min)() << endl;
	cout << "size_t: \t" << "所占字节数：" << sizeof(size_t);
	cout << "\t最大值：" << (numeric_limits<size_t>::max)();
	cout << "\t最小值：" << (numeric_limits<size_t>::min)() << endl;
	cout << "string: \t" << "所占字节数：" << sizeof(string) << endl;
	// << "\t最大值：" << (numeric_limits<string>::max)() << "\t最小值：" << (numeric_limits<string>::min)() << endl;  
	cout << "type: \t\t" << "************size**************" << endl;
	typedef int feed;
	feed b = 111;
	cout << b << endl;
	feed c = 072;
	cout << c << endl;
}




// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件