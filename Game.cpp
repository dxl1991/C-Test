#include <iostream>
#include <time.h>
#include "Game.h"
using namespace std;

Game::Game()
{
	srand((int)time(0));
	this->score = 0;
	for (int i = 0; i < _X; i++)
	{
		for (int j = 0; j < _Y; j++)
		{
			sprites[i][j] = Sprite(randomType(),i,j);
			do 
			{
				sprites[i][j] = Sprite(randomType(), i, j);
			} while (canEliminateX(i, j) || canEliminateY(i, j) || canEliminateXY(i,j));
		}
	}
	print();
}
Game::~Game() 
{
	delete NULL_SPRITE;
	NULL_SPRITE = NULL;
}

//消除
bool Game::EliminateSprite(int x,int y)
{
	bool flag_x = canEliminateX(x, y);
	bool flag_y = canEliminateY(x, y);
	bool flag_xy = canEliminateXY(x, y);
	set<Sprite*> spriteSet;
	if (flag_x)
	{
		findEliminateSprite(x, y, &spriteSet, true);
	}
	if (flag_y)
	{
		findEliminateSprite(x, y, &spriteSet, false);
	}
	if (flag_xy)
	{
		findEliminateSpriteXY(x, y, &spriteSet);
	}

	if (spriteSet.size() == 0)
	{
		return false;
	}

	this->score += spriteSet.size();
	set<Sprite*>::iterator it;
	for (it = spriteSet.begin(); it != spriteSet.end(); it++)
	{
		Sprite* sprite = *it;
		sprite->type = ZERO;

	}
	spriteSet.clear();
	print();
	moveSprite(&spriteSet);
	print();
	//判断是否可以再消除
	for (it = spriteSet.begin(); it != spriteSet.end(); it++)
	{
		Sprite* sprite = *it;
		EliminateSprite(sprite->x,sprite->y);
	}
	return true;
}

//坐标交换
bool Game::swapSprite(int a, int b)
{
	if (a == b)
	{
		cout << "不能输入相同坐标";
		return false;
	}
	int x1 = a / _Y;
	int y1 = a % _Y;
	int x2 = b / _Y;
	int y2 = b % _Y;

	if (x1 < 0 || x1 >= _X || y1 < 0 || y1 >= _Y || x2 < 0 || x2 >= _X || y2 < 0 || y2 >= _Y)
	{
		cout << "坐标输入错误，超出边界" << endl;
		return false;
	}

	if ((x1 == x2 && abs(y1 - y2) == 1) || (y1 == y2 && abs(x1 - x2) == 1))
	{
		SpriteType temp = sprites[x1][y1].type;
		sprites[x1][y1].type = sprites[x2][y2].type;
		sprites[x2][y2].type = temp;

		bool flag1 = EliminateSprite(x1, y1);
		bool flag2 = EliminateSprite(x2, y2);

		if (flag1 || flag2)
		{
			return true;
		}
		else 
		{
			sprites[x2][y2].type = sprites[x1][y1].type;
			sprites[x1][y1].type = temp;
			cout << "坐标交换后没什么变化" << endl;
			return false;
		}
	}
	else
	{
		cout << "只有相邻的坐标才能交换" << endl;
		return false;
	}
}
//收集田字元素
void Game::findEliminateSpriteXY(int x, int y, set<Sprite*>* spriteSet)
{
	Sprite self = sprites[x][y];
	Sprite* upSprite = getUpSprite(x, y);
	Sprite* downSprite = getDownSprite(x, y);
	Sprite* leftSprite = getLeftSprite(x, y);
	Sprite* rightSprite = getRightSprite(x, y);

	if (upSprite->type == self.type)
	{
		if (leftSprite->type == self.type)
		{
			Sprite* sprite = getUpSprite(leftSprite->x, leftSprite->y);
			if (sprite->type == self.type)
			{
				if (spriteSet->find(&self) == spriteSet->end())
				{
					spriteSet->insert(&self);
				}
				if (spriteSet->find(upSprite) == spriteSet->end())
				{
					spriteSet->insert(upSprite);
				}
				if (spriteSet->find(leftSprite) == spriteSet->end())
				{
					spriteSet->insert(leftSprite);
				}
				if (spriteSet->find(sprite) == spriteSet->end())
				{
					spriteSet->insert(sprite);
				}
			}
		}
		if (rightSprite->type == self.type)
		{
			Sprite* sprite = getUpSprite(rightSprite->x, rightSprite->y);
			if (sprite->type == self.type)
			{
				if (spriteSet->find(&self) == spriteSet->end())
				{
					spriteSet->insert(&self);
				}
				if (spriteSet->find(upSprite) == spriteSet->end())
				{
					spriteSet->insert(upSprite);
				}
				if (spriteSet->find(rightSprite) == spriteSet->end())
				{
					spriteSet->insert(rightSprite);
				}
				if (spriteSet->find(sprite) == spriteSet->end())
				{
					spriteSet->insert(sprite);
				}
			}
		}
	}
	if (downSprite->type == self.type)
	{
		if (leftSprite->type == self.type)
		{
			Sprite* sprite = getDownSprite(leftSprite->x, leftSprite->y);
			if (sprite->type == self.type)
			{
				if (spriteSet->find(&self) == spriteSet->end())
				{
					spriteSet->insert(&self);
				}
				if (spriteSet->find(downSprite) == spriteSet->end())
				{
					spriteSet->insert(downSprite);
				}
				if (spriteSet->find(leftSprite) == spriteSet->end())
				{
					spriteSet->insert(leftSprite);
				}
				if (spriteSet->find(sprite) == spriteSet->end())
				{
					spriteSet->insert(sprite);
				}
			}
		}
		if (rightSprite->type == self.type)
		{
			Sprite* sprite = getDownSprite(rightSprite->x, rightSprite->y);
			if (sprite->type == self.type)
			{
				if (spriteSet->find(&self) == spriteSet->end())
				{
					spriteSet->insert(&self);
				}
				if (spriteSet->find(downSprite) == spriteSet->end())
				{
					spriteSet->insert(downSprite);
				}
				if (spriteSet->find(rightSprite) == spriteSet->end())
				{
					spriteSet->insert(rightSprite);
				}
				if (spriteSet->find(sprite) == spriteSet->end())
				{
					spriteSet->insert(sprite);
				}
			}
		}
	}
}
//收集同一方向，相邻且值一样的元素
void Game::findEliminateSprite(int x, int y,set<Sprite*> *spriteSet,bool directX)
{
	Sprite self = sprites[x][y];
	Sprite* upSprite = getUpSprite(x, y);
	Sprite* downSprite = getDownSprite(x, y);
	Sprite* leftSprite = getLeftSprite(x, y);
	Sprite* rightSprite = getRightSprite(x, y);

    //找竖向相同的
	if (!directX)
	{
		if (upSprite->type == self.type && spriteSet->find(upSprite) == spriteSet->end())
		{
			spriteSet->insert(upSprite);
			findEliminateSprite(upSprite->x, upSprite->y, spriteSet,false);
		}

		if (downSprite->type == self.type && spriteSet->find(downSprite) == spriteSet->end())
		{
			spriteSet->insert(downSprite);
			findEliminateSprite(downSprite->x, downSprite->y, spriteSet, false);
		}
	}
	else 
	{
		//找横向相同的
		if (leftSprite->type == self.type && spriteSet->find(leftSprite) == spriteSet->end())
		{
			spriteSet->insert(leftSprite);
			findEliminateSprite(leftSprite->x, leftSprite->y, spriteSet,true);
		}

		if (rightSprite->type == self.type && spriteSet->find(rightSprite) == spriteSet->end())
		{
			spriteSet->insert(rightSprite);
			findEliminateSprite(rightSprite->x, rightSprite->y, spriteSet, true);
		}
	}
}

//田字能否消除
bool Game::canEliminateXY(int x, int y)
{
	Sprite self = sprites[x][y];
	Sprite* upSprite = getUpSprite(x, y);
	Sprite* downSprite = getDownSprite(x, y);
	Sprite* leftSprite = getLeftSprite(x, y);
	Sprite* rightSprite = getRightSprite(x, y);
	if (upSprite->type == self.type)
	{
		if (leftSprite->type == self.type)
		{
			Sprite* sprite = getUpSprite(leftSprite->x, leftSprite->y);
			if (sprite->type == self.type)
			{
				return true;
			}
		}
		if (rightSprite->type == self.type)
		{
			Sprite* sprite = getUpSprite(rightSprite->x, rightSprite->y);
			if (sprite->type == self.type)
			{
				return true;
			}
		}
	}
	if (downSprite->type == self.type)
	{
		if (leftSprite->type == self.type)
		{
			Sprite* sprite = getDownSprite(leftSprite->x, leftSprite->y);
			if (sprite->type == self.type)
			{
				return true;
			}
		}
		if (rightSprite->type == self.type)
		{
			Sprite* sprite = getDownSprite(rightSprite->x, rightSprite->y);
			if (sprite->type == self.type)
			{
				return true;
			}
		}
	}
	return false;
}

//竖向能否消除
bool Game::canEliminateY(int x,int y)
{
	Sprite self = sprites[x][y];
	Sprite* upSprite = getUpSprite(x, y);
	Sprite* downSprite = getDownSprite(x, y);
	if (upSprite->type == self.type)
	{
		if (downSprite->type == self.type)
		{
			return true;
		}
		Sprite* sprite = getUpSprite(upSprite->x, upSprite->y);
		if (sprite->type == self.type)
		{
			return true;
		}
	}
	if (downSprite->type == self.type)
	{
		Sprite* sprite = getDownSprite(downSprite->x, downSprite->y);
		if (sprite->type == self.type)
		{
			return true;
		}
	}
	return false;
}

//横向能否消除
bool Game::canEliminateX(int x, int y)
{
	Sprite self = sprites[x][y];
	Sprite* leftSprite = getLeftSprite(x, y);
	Sprite* rightSprite = getRightSprite(x, y);
	if (leftSprite->type == self.type)
	{
		if (rightSprite->type == self.type)
		{
			return true;
		}
		Sprite* sprite = getLeftSprite(leftSprite->x, leftSprite->y);
		if (sprite->type == self.type)
		{
			return true;
		}
	}
	if (rightSprite->type == self.type)
	{
		Sprite* sprite = getRightSprite(rightSprite->x, rightSprite->y);
		if (sprite->type == self.type)
		{
			return true;
		}
	}
	return false;
}
//元素下落
void Game::moveSprite(set<Sprite*> *spriteSet)
{
	for (int i = _X - 1; i >= 0; i--)
	{
		for (int j = 0; j < _Y; j++)
		{
			if (sprites[i][j].type == ZERO)
			{
				int tempX = i - 1;
				while (tempX >= 0 && sprites[tempX][j].type == ZERO)
				{
					tempX--;
				}
				
				if (tempX >= 0 && sprites[tempX][j].type != ZERO)
				{
					sprites[i][j].type = sprites[tempX][j].type;
					sprites[tempX][j].type = ZERO;
					spriteSet->insert(&sprites[i][j]);
					spriteSet->insert(&sprites[tempX][j]);
				}
				else 
				{
					sprites[i][j].type = randomType();
					spriteSet->insert(&sprites[i][j]);
				}
			}
		}
	}
}

Sprite* Game::getUpSprite(int x, int y)
{
	if (x - 1 < 0) {
		return NULL_SPRITE;
	}
	return &sprites[x - 1][y];
}

Sprite* Game::getDownSprite(int x, int y)
{
	if (x + 1 >= _X) {
		return NULL_SPRITE;
	}
	return &sprites[x + 1][y];
}

Sprite* Game::getLeftSprite(int x, int y)
{
	if (y - 1 < 0) {
		return NULL_SPRITE;
	}
	return &sprites[x][y - 1];
}

Sprite* Game::getRightSprite(int x, int y)
{
	if (y + 1 >= _Y) {
		return NULL_SPRITE;
	}
	return &sprites[x][y + 1];
}

SpriteType Game::randomType()
{
	int number = rand() % FIVE + 1;
	return (SpriteType)number;
}

void Game::print()
{
	cout << endl << "score: " << score << endl;
	for (int i=0;i < _X;i++)
	{
		cout << endl;
		for (int j=0;j < _Y;j++)
		{
			cout << " " << sprites[i][j].type;
		}
	}
	cout << endl;
}