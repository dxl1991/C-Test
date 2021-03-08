#pragma once
#include "SpriteType.h"
class Sprite
{
public:
	Sprite();
	Sprite(SpriteType type,int x,int y);
	SpriteType type;
	int x, y;
	bool operator == (const Sprite& _A) const
	{
		return x == _A.x && y == _A.y;
	}
	bool operator < (const Sprite& _A) const
	{
		if (x < _A.x)
		{
			return true;
		}
		if (x == _A.x)
		{
			if (y < _A.y)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		return false;
	}
};


