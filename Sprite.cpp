#include "Sprite.h"

Sprite::Sprite()
{
	this->type = ZERO;
	this->x = 0;
	this->y = 0;
}

Sprite::Sprite(SpriteType type,int x, int y)
{
	this->type = type;
	this->x = x;
	this->y = y;
}