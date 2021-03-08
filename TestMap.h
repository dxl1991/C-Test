#pragma once
#include <map>
#include <string>
#include "Line.h"
class TestMap
{
public:
	std::map<int, string> personnel;
	using PersonMap = std::map<int, string>;
	void testMap();
};

typedef struct tagStudentinfo
{
	int niD;
	string name;
	bool operator < (tagStudentinfo const& _A) const
	{
		if (niD < _A.niD)
		{
			return true;
		}
		else if (niD == _A.niD)
		{
			return name.compare(_A.name) < 0;
		}
		return false;
	}

}Studentinfo,*PStrudentinfo;

