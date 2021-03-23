#pragma once
#include "SpriteType.h"
class Sprite
{
public:
	Sprite();
	Sprite(SpriteType type, int x, int y);
	SpriteType type;
	int x, y;
	bool operator == (const Sprite& _A) const
	{
		return x == _A.x && y == _A.y;
	}
	bool operator < (const Sprite& _A) const
	{
		if (_A.type == type)
		{
			if (_A.x == x)
			{
				return _A.y < y;
			}
			return _A.x < x;
		}
		return _A.type < type;
	}
};


