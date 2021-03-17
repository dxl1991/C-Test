#include <iostream>
#include <time.h>
#include <algorithm>
#include "Game.h"
#include "windows.h"
using namespace std;

Game::Game()
{
	Game(6,6);
}
Game::Game(int x,int y) : score(0)
{
	_X = x;
	_Y = y;
	sprites = new Sprite * [_X];
	srand((int)time(0));
	for (int i = 0; i < _X; i++)
	{
		sprites[i] = new Sprite[_Y];
	}
	do
	{
		for (int i = 0; i < _X; i++)
		{
			for (int j = 0; j < _Y; j++)
			{
				do
				{
					sprites[i][j] = Sprite(randomType(), i, j);
				} while (canEliminateX(i, j) || canEliminateY(i, j) || canEliminateXY(i, j));
			}
		}
	} while (noEliminate());
	printHelpInfo();
	print();
}
Game::~Game() 
{
	delete NULL_SPRITE;
	NULL_SPRITE = NULL;
	for (int i = 0; i < _X; i++)
	{
		delete[] sprites[i];
	}
	delete[] sprites;
}

//消除
bool Game::EliminateSprite(int x,int y,bool useProp,SpriteType type)
{
	SpriteSet spriteSet;
	findEliminateSpriteAll(x,y,spriteSet);
	bool prop = false;
	if (sprites[x][y].type > FIVE && useProp)
	{
		spriteSet.insert(&sprites[x][y]);
		prop = true;
	}

	if (spriteSet.size() == 0)
	{
		return false;
	}
	SpriteSet spriteSetTemp = spriteSet;
	if (prop)
	{
	   findPropEliminate(spriteSet,type);
	}
	SpriteSet::iterator it;
	for (it = spriteSet.begin(); it != spriteSet.end(); it++)
	{
		Sprite* sprite = *it;
		sprite->type = ZERO;
	}
	this->score += spriteSet.size();
	SpriteType spriteType = generateProp(spriteSetTemp);
	if (spriteType > FIVE)
	{
		sprites[x][y].type = spriteType;
		if (propStatistics.find(spriteType) == propStatistics.end())
		{
			propStatistics[spriteType] = 1;
		}
		else
		{
			propStatistics[spriteType] = propStatistics[spriteType] + 1;
		}
		
	}
	return true;
}
//产生特殊元素
SpriteType Game::generateProp(SpriteSet& spriteSet)
{
	map<int, int> xCount, yCount;
	SpriteSet::iterator it;
	for (it = spriteSet.begin(); it != spriteSet.end(); it++)
	{
		Sprite* sprite = *it;
		map<int, int>::iterator iter = xCount.find(sprite->x);
		if (iter == xCount.end())
		{
			xCount[sprite->x] = 1;
		}
		else
		{
			xCount[sprite->x] = xCount[sprite->x] + 1;
		}
		iter = yCount.find(sprite->y);
		if (iter == yCount.end())
		{
			yCount[sprite->y] = 1;
		}
		else
		{
			yCount[sprite->y] = yCount[sprite->y] + 1;
		}
	}
	if (!forbidPlane)
	{
		if (xCount.size() == 2 && yCount.size() == 2 && spriteSet.size() == 4)
		{
			return PLANE;
		}
		if (((xCount.size() == 2 && yCount.size() == 3) || (xCount.size() == 3 && yCount.size() == 2)) && spriteSet.size() == 5)
		{
			return PLANE;
		}
	}

	SpriteType spriteType = ZERO;
	map<int, int>::iterator iter;
	for (iter = xCount.begin(); iter != xCount.end(); iter++)
	{
		if (iter->second == 4)
		{
			spriteType = ALL_Y;
		}
	}
	for (iter = yCount.begin(); iter != yCount.end(); iter++)
	{
		if (iter->second == 4)
		{
			spriteType = ALL_X;
		}
	}
	int count = spriteSet.size();
	if (count >= 6)
	{
		spriteType = ALL;
	}
	else if (count == 5)
	{
		spriteType = BOOM;
	}
	return spriteType;
}
//两个特殊元素交换
void Game::EliminateSprite1(Sprite* sprite1, Sprite* sprite2)
{
	SpriteSet spriteSet;
	if (sprite1->type == ALL && sprite2->type == ALL)
	{
		for (int i = 0; i < _X; i++)
		{
			for (int j = 0; j < _Y; j++)
			{
				spriteSet.insert(&sprites[i][j]);
			}
		}
		sprite1->type = ZERO;
		sprite2->type = ZERO;
	}
	if ((sprite1->type == ALL && sprite2->type != ALL) || (sprite1->type != ALL && sprite2->type == ALL))
	{
		SpriteType type;
		if (sprite1->type ==ALL)
		{
			sprite1->type = ZERO;
			type = sprite2->type;
		}
		else 
		{
			sprite2->type = ZERO;
			type = sprite1->type;
		}
		spriteSet.insert(sprite1);
		spriteSet.insert(sprite2);
		SpriteType rType = randomType();
		for (int i = 0; i < _X; i++)
		{
			for (int j = 0; j < _Y; j++)
			{
				if (sprites[i][j].type == rType)
				{
					sprites[i][j].type = (type == ALL_X || type == ALL_Y) ? ((rand() % 2 == 0) ? ALL_X : ALL_Y) : type;
					spriteSet.insert(&sprites[i][j]);
				}
			}
		}
		print();
	}
	if ((sprite1->type == BOOM && (sprite2->type == ALL_X || sprite2->type == ALL_Y)) || (sprite2->type == BOOM && (sprite1->type == ALL_X || sprite1->type == ALL_Y)))
	{
		int min_x, max_x, min_y, max_y;
		min_x = max(sprite2->x - 1,0);
		max_x = min(sprite2->x + 1, _X-1);
		min_y = max(sprite2->y - 1, 0);
		max_y = min(sprite2->y + 1, _Y-1);
		for (int i = min_x; i <= max_x; i++)
		{
			for (int j = 0; j < _Y; j++)
			{
				spriteSet.insert(&sprites[i][j]);
			}
		}
		for (int i = 0; i < _X; i++)
		{
			for (int j = min_y; j <= max_y; j++)
			{
				spriteSet.insert(&sprites[i][j]);
			}
		}
		sprite1->type = ZERO;
		sprite2->type = ZERO;
	}
	if (sprite1->type == BOOM && sprite2->type == BOOM)
	{
		for (int i = max(0, sprite2->x - 2); i <= min(_X - 1, sprite2->x + 2); i++)
		{
			for (int j = max(0, sprite2->y - 2); j <= min(_Y - 1, sprite2->y + 2); j++)
			{
				spriteSet.insert(&sprites[i][j]);
			}
		}
		sprite1->type = ZERO;
		sprite2->type = ZERO;
	}
	if ((sprite1->type == ALL_X || sprite1->type == ALL_Y) && (sprite2->type == ALL_X || sprite2->type == ALL_Y))
	{
		for (int i = 0; i < _X; i++)
		{
			spriteSet.insert(&sprites[i][sprite2->y]);
		}

		for (int j = 0; j < _Y; j++)
		{
			spriteSet.insert(&sprites[sprite2->x][j]);
		}
		sprite1->type = ZERO;
		sprite2->type = ZERO;
	}
	if (sprite1->type == PLANE || sprite2->type == PLANE)
	{
		spriteSet.insert(sprite1);
		spriteSet.insert(sprite2);
	}
	findPropEliminate(spriteSet, randomType());
	SpriteSet::iterator it;
	for (it = spriteSet.begin(); it != spriteSet.end(); it++)
	{
		Sprite* sprite = *it;
		sprite->type = ZERO;
	}
	this->score += spriteSet.size();
	spriteSet.clear();
	print();
	moveSprite();
}
//一个特殊元素和一个普通元素交换
void Game::EliminateSprite2(Sprite* sprite1, Sprite* sprite2)
{
	int x = sprite1->type <= FIVE ? sprite1->x : sprite2->x;
	int y = sprite1->type <= FIVE ? sprite1->y : sprite2->y;
	SpriteSet spriteSet1;
	findEliminateSpriteAll(x, y, spriteSet1);
	SpriteSet spriteSet2;
	if (sprite1->type > FIVE)
	{
		spriteSet2.insert(sprite1);
	    findPropEliminate(spriteSet2, sprite2->type);
	}
	else 
	{
		spriteSet2.insert(sprite2);
		findPropEliminate(spriteSet2, sprite1->type);
	}
	if (spriteSet1.size() > 0)
	{
		SpriteSet::iterator it;
		for (it = spriteSet1.begin(); it != spriteSet1.end(); it++)
		{
			spriteSet2.insert(*it);
		}
	}
	SpriteSet::iterator it;
	for (it = spriteSet2.begin(); it != spriteSet2.end(); it++)
	{
		Sprite* sprite = *it;
		sprite->type = ZERO;
	}
	if (spriteSet1.size() > 0)
	{
		SpriteType spriteType = generateProp(spriteSet1);
		if (spriteType > FIVE)
		{
			sprites[x][y].type = spriteType;
			if (propStatistics.find(spriteType) == propStatistics.end())
			{
				propStatistics[spriteType] = 1;
			}
			else
			{
				propStatistics[spriteType] = propStatistics[spriteType] + 1;
			}
		}
	}
	this->score += spriteSet2.size();
	print();
	moveSprite();
}

//两个普通元素交换
bool Game::EliminateSprite3(Sprite* sprite1, Sprite* sprite2)
{
	bool flag = false;
	SpriteSet spriteSet1;
	findEliminateSpriteAll(sprite1->x, sprite1->y, spriteSet1);
	if (spriteSet1.size() > 0)
	{
		flag = true;
		SpriteSet::iterator it;
		for (it = spriteSet1.begin(); it != spriteSet1.end(); it++)
		{
			Sprite* sprite = *it;
			sprite->type = ZERO;
		}
		this->score += spriteSet1.size();
		SpriteType spriteType = generateProp(spriteSet1);
		if (spriteType > FIVE)
		{
			sprites[sprite1->x][sprite1->y].type = spriteType;
			if (propStatistics.find(spriteType) == propStatistics.end())
			{
				propStatistics[spriteType] = 1;
			}
			else
			{
				propStatistics[spriteType] = propStatistics[spriteType] + 1;
			}
		}
	}

	SpriteSet spriteSet2;
	findEliminateSpriteAll(sprite2->x, sprite2->y, spriteSet2);
	if (spriteSet2.size() > 0)
	{
		flag = true;
		SpriteSet::iterator it;
		for (it = spriteSet2.begin(); it != spriteSet2.end(); it++)
		{
			Sprite* sprite = *it;
			sprite->type = ZERO;
		}
		this->score += spriteSet2.size();
		SpriteType spriteType = generateProp(spriteSet2);
		if (spriteType > FIVE)
		{
			sprites[sprite2->x][sprite2->y].type = spriteType;
			if (propStatistics.find(spriteType) == propStatistics.end())
			{
				propStatistics[spriteType] = 1;
			}
			else
			{
				propStatistics[spriteType] = propStatistics[spriteType] + 1;
			}
		}
	}
	if (!flag)
	{
		return false;
	}
	print();
	moveSprite();
	return true;
}

//坐标交换
bool Game::swapSprite(int a, int b)
{
	a--;
	b--;
	int x1 = a / _Y;
	int y1 = a % _Y;
	int x2 = b / _Y;
	int y2 = b % _Y;

	if (x1 < 0 || x1 >= _X || y1 < 0 || y1 >= _Y || x2 < 0 || x2 >= _X || y2 < 0 || y2 >= _Y)
	{
		cout << "坐标输入错误，超出边界" << endl;
		return false;
	}
	//双击
	if (a == b)
	{
		if (sprites[x1][y1].type > FIVE)
		{
			EliminateSprite(x1, y1, true,randomType());
			print();
			moveSprite();
			return true;
		}

		cout << "不能输入相同坐标";
		return false;
	}

	if ((x1 == x2 && abs(y1 - y2) == 1) || (y1 == y2 && abs(x1 - x2) == 1))
	{
		//两个特殊元素交换
		if (sprites[x1][y1].type > FIVE && sprites[x2][y2].type > FIVE)
		{
			EliminateSprite1(&sprites[x1][y1], &sprites[x2][y2]);
			return true;
		}
		SpriteType temp = sprites[x1][y1].type;
		sprites[x1][y1].type = sprites[x2][y2].type;
		sprites[x2][y2].type = temp;
		//一个特殊元素和普通元素交换
		if (sprites[x1][y1].type > FIVE || sprites[x2][y2].type > FIVE)
		{
			EliminateSprite2(&sprites[x1][y1], &sprites[x2][y2]);
			return true;
		}
		//两个普通元素交换
		if (!EliminateSprite3(&sprites[x1][y1], &sprites[x2][y2]))
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
//收集周围能消除的元素
void Game::findEliminateSpriteAll(int x, int y, SpriteSet& spriteSet)
{
	bool flag_x = canEliminateX(x, y);
	bool flag_y = canEliminateY(x, y);
	bool flag_xy = canEliminateXY(x, y);
	if (flag_x)
	{
		findEliminateSprite(x, y, spriteSet, true);
	}
	if (flag_y)
	{
		findEliminateSprite(x, y, spriteSet, false);
	}
	if (flag_xy)
	{
		findEliminateSpriteXY(x, y, spriteSet);
	}
}
//收集田字元素
void Game::findEliminateSpriteXY(int x, int y, SpriteSet& spriteSet)
{
	Sprite* self = &sprites[x][y];
	Sprite* upSprite = getUpSprite(x, y);
	Sprite* downSprite = getDownSprite(x, y);
	Sprite* leftSprite = getLeftSprite(x, y);
	Sprite* rightSprite = getRightSprite(x, y);

	if (upSprite->type == self->type)
	{
		if (leftSprite->type == self->type)
		{
			Sprite* sprite = getUpSprite(leftSprite->x, leftSprite->y);
			if (sprite->type == self->type)
			{
				spriteSet.insert(self);
				spriteSet.insert(upSprite);
				spriteSet.insert(leftSprite);
				spriteSet.insert(sprite);
			}
		}
		if (rightSprite->type == self->type)
		{
			Sprite* sprite = getUpSprite(rightSprite->x, rightSprite->y);
			if (sprite->type == self->type)
			{
				spriteSet.insert(self);
				spriteSet.insert(upSprite);
				spriteSet.insert(rightSprite);
				spriteSet.insert(sprite);
			}
		}
	}
	if (downSprite->type == self->type)
	{
		if (leftSprite->type == self->type)
		{
			Sprite* sprite = getDownSprite(leftSprite->x, leftSprite->y);
			if (sprite->type == self->type)
			{
				spriteSet.insert(self);
				spriteSet.insert(downSprite);
				spriteSet.insert(leftSprite);
				spriteSet.insert(sprite);
			}
		}
		if (rightSprite->type == self->type)
		{
			Sprite* sprite = getDownSprite(rightSprite->x, rightSprite->y);
			if (sprite->type == self->type)
			{
				spriteSet.insert(self);
				spriteSet.insert(downSprite);
				spriteSet.insert(rightSprite);
				spriteSet.insert(sprite);
			}
		}
	}
}
//收集同一方向，相邻且值一样的元素
void Game::findEliminateSprite(int x, int y,SpriteSet& spriteSet,bool directX)
{
	Sprite self = sprites[x][y];
	Sprite* upSprite = getUpSprite(x, y);
	Sprite* downSprite = getDownSprite(x, y);
	Sprite* leftSprite = getLeftSprite(x, y);
	Sprite* rightSprite = getRightSprite(x, y);

    //找竖向相同的
	if (!directX)
	{
		if (upSprite->type != ZERO && upSprite->type == self.type && spriteSet.find(upSprite) == spriteSet.end())
		{
			spriteSet.insert(upSprite);
			findEliminateSprite(upSprite->x, upSprite->y, spriteSet,false);
		}

		if (downSprite->type != ZERO && downSprite->type == self.type && spriteSet.find(downSprite) == spriteSet.end())
		{
			spriteSet.insert(downSprite);
			findEliminateSprite(downSprite->x, downSprite->y, spriteSet, false);
		}
	}
	else 
	{
		//找横向相同的
		if (leftSprite->type != ZERO && leftSprite->type == self.type && spriteSet.find(leftSprite) == spriteSet.end())
		{
			spriteSet.insert(leftSprite);
			findEliminateSprite(leftSprite->x, leftSprite->y, spriteSet,true);
		}

		if (rightSprite->type != ZERO && rightSprite->type == self.type && spriteSet.find(rightSprite) == spriteSet.end())
		{
			spriteSet.insert(rightSprite);
			findEliminateSprite(rightSprite->x, rightSprite->y, spriteSet, true);
		}
	}
}

//田字能否消除
bool Game::canEliminateXY(int x, int y)
{
	if (forbidPlane)
	{
		return false;
	}
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
SpriteType Game::tryEliminate(int x, int y) 
{
	SpriteSet spriteSet;
	findEliminateSpriteAll(x, y, spriteSet);
	if (spriteSet.size() == 0)
	{
		return ZERO;
	}
	return generateProp(spriteSet);
}
bool Game::EliminateSprite(SpriteSet& spriteSet)
{
	SpriteSet sortSet;
	bool flag = false;
	SpriteSet::iterator it;
	for (it = spriteSet.begin(); it != spriteSet.end(); it++)
	{
		Sprite* sprite = *it;
		SpriteType type = tryEliminate(sprite->x, sprite->y);
		sortSet.insert(new Sprite(type,sprite->x, sprite->y));
	}
	for (it = sortSet.begin(); it != sortSet.end(); it++)
	{
		Sprite* sprite = *it;
		if (EliminateSprite(sprite->x, sprite->y, false, randomType()))
		{
			flag = true;
		}
		delete sprite;
	}
	return flag;
}
//元素下落
void Game::moveSprite()
{
	SpriteSet spriteSet;
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
					spriteSet.insert(&sprites[i][j]);
					spriteSet.insert(&sprites[tempX][j]);
				}
				else 
				{
					sprites[i][j].type = randomWightType();
					spriteSet.insert(&sprites[i][j]);
				}
			}
		}
	}
	print();
	if (EliminateSprite(spriteSet))
	{
		print();
		moveSprite();
	}
}
//是否没有可消除的了
bool Game::noEliminate()
{
	for (int i=0;i<_X;i++)
	{
		for (int j=0;j<_Y;j++)
		{
			if (sprites[i][j].type > FIVE)
			{
				return false;
			}
			Sprite* downSprite = getDownSprite(i, j);
			Sprite* rightSprite = getRightSprite(i, j);
			if (downSprite->type != ZERO)
			{
				SpriteType tempType = sprites[i][j].type;
				sprites[i][j].type = downSprite->type;
				downSprite->type = tempType;
				if (canEliminateX(i, j) || canEliminateY(i, j) || canEliminateXY(i, j) || 
					canEliminateX(downSprite->x, downSprite->y) || canEliminateY(downSprite->x, downSprite->y) || canEliminateXY(downSprite->x, downSprite->y))
				{
					downSprite->type = sprites[i][j].type;
					sprites[i][j].type = tempType;
					return false;
				}
				downSprite->type = sprites[i][j].type;
				sprites[i][j].type = tempType;
			}
			if (rightSprite->type != ZERO)
			{
				SpriteType tempType = sprites[i][j].type;
				sprites[i][j].type = rightSprite->type;
				rightSprite->type = tempType;
				if (canEliminateX(i, j) || canEliminateY(i, j) || canEliminateXY(i, j) ||
					canEliminateX(rightSprite->x, rightSprite->y) || canEliminateY(rightSprite->x, rightSprite->y) || canEliminateXY(rightSprite->x, rightSprite->y))
				{
					rightSprite->type = sprites[i][j].type;
					sprites[i][j].type = tempType;
					return false;
				}
				rightSprite->type = sprites[i][j].type;
				sprites[i][j].type = tempType;
			}
		}
	}
	return true;
}

void Game::findSuggest(int& a, int& b)
{
	a = 0, b = 0;
	SpriteType bestType = ZERO;
	bool propEliminate = false;
	bool canGenerateProp = false;
	for (int i = _X - 1; i >= 0; i--)
	{
		for (int j = _Y - 1; j >= 0; j--)
		{
			Sprite* downSprite = getDownSprite(i, j);
			Sprite* rightSprite = getRightSprite(i, j);
			if (sprites[i][j].type > FIVE)
			{
				if (a == 0 && b == 0)
				{
					a = i * _Y + j + 1;
					b = i * _Y + j + 1;
					if (downSprite->type > FIVE)
					{
						b = downSprite->x * _Y + downSprite->y + 1;
					}
					if (rightSprite->type > FIVE)
					{
						b = rightSprite->x * _Y + rightSprite->y + 1;
					}
					propEliminate = true;
				}
				continue;
			}
			if (downSprite->type > ZERO && downSprite->type <= FIVE)
			{
				SpriteType tempType = sprites[i][j].type;
				sprites[i][j].type = downSprite->type;
				downSprite->type = tempType;
				bool flag_x = canEliminateX(i, j);
				bool flag_y = canEliminateY(i, j);
				bool flag_xy = canEliminateXY(i, j);
				SpriteSet spriteSet;
				if (flag_x)
				{
					findEliminateSprite(i, j, spriteSet, true);
				}
				if (flag_y)
				{
					findEliminateSprite(i, j, spriteSet, false);
				}
				if (flag_xy)
				{
					findEliminateSpriteXY(i, j, spriteSet);
				}
				if (spriteSet.size() > 0)
				{
					if (((a == 0 && b == 0) || propEliminate) && !canGenerateProp)
					{
						a = i * _Y + j + 1;
						b = downSprite->x * _Y + downSprite->y + 1;
					}
					SpriteType spriteType = generateProp(spriteSet);
					if (spriteType > FIVE && spriteType > bestType)
					{
						bestType = spriteType;
						a = i * _Y + j + 1;
						b = downSprite->x * _Y + downSprite->y + 1;
						canGenerateProp = true;
					}
				}
				flag_x = canEliminateX(downSprite->x, downSprite->y);
				flag_y = canEliminateY(downSprite->x, downSprite->y);
				flag_xy = canEliminateXY(downSprite->x, downSprite->y);
				spriteSet.clear();
				if (flag_x)
				{
					findEliminateSprite(downSprite->x, downSprite->y, spriteSet, true);
				}
				if (flag_y)
				{
					findEliminateSprite(downSprite->x, downSprite->y, spriteSet, false);
				}
				if (flag_xy)
				{
					findEliminateSpriteXY(downSprite->x, downSprite->y, spriteSet);
				}
				if (spriteSet.size() > 0)
				{
					if (((a == 0 && b == 0) || propEliminate) && !canGenerateProp)
					{
						a = i * _Y + j + 1;
						b = downSprite->x * _Y + downSprite->y + 1;
					}
					SpriteType spriteType = generateProp(spriteSet);
					if (spriteType > FIVE && spriteType > bestType)
					{
						bestType = spriteType;
						a = i * _Y + j + 1;
						b = downSprite->x * _Y + downSprite->y + 1;
						canGenerateProp = true;
					}
				}
				downSprite->type = sprites[i][j].type;
				sprites[i][j].type = tempType;
			}
			if (rightSprite->type > ZERO && rightSprite->type <= FIVE)
			{
				SpriteType tempType = sprites[i][j].type;
				sprites[i][j].type = rightSprite->type;
				rightSprite->type = tempType;
				bool flag_x = canEliminateX(i, j);
				bool flag_y = canEliminateY(i, j);
				bool flag_xy = canEliminateXY(i, j);
				SpriteSet spriteSet;
				if (flag_x)
				{
					findEliminateSprite(i, j, spriteSet, true);
				}
				if (flag_y)
				{
					findEliminateSprite(i, j, spriteSet, false);
				}
				if (flag_xy)
				{
					findEliminateSpriteXY(i, j, spriteSet);
				}
				if (spriteSet.size() > 0)
				{
					if (((a == 0 && b == 0) || propEliminate) && !canGenerateProp)
					{
						a = i * _Y + j + 1;
						b = rightSprite->x * _Y + rightSprite->y + 1;
					}
					SpriteType spriteType = generateProp(spriteSet);
					if (spriteType > FIVE && spriteType > bestType)
					{
						bestType = spriteType;
						a = i * _Y + j + 1;
						b = rightSprite->x * _Y + rightSprite->y + 1;
						canGenerateProp = true;
					}
				}
				flag_x = canEliminateX(rightSprite->x, rightSprite->y);
				flag_y = canEliminateY(rightSprite->x, rightSprite->y);
				flag_xy = canEliminateXY(rightSprite->x, rightSprite->y);
				spriteSet.clear();
				if (flag_x)
				{
					findEliminateSprite(rightSprite->x, rightSprite->y, spriteSet, true);
				}
				if (flag_y)
				{
					findEliminateSprite(rightSprite->x, rightSprite->y, spriteSet, false);
				}
				if (flag_xy)
				{
					findEliminateSpriteXY(rightSprite->x, rightSprite->y, spriteSet);
				}
				if (spriteSet.size() > 0)
				{
					if (((a == 0 && b == 0) || propEliminate) && !canGenerateProp)
					{
						a = i * _Y + j + 1;
						b = rightSprite->x * _Y + rightSprite->y + 1;
					}
					SpriteType spriteType = generateProp(spriteSet);
					if (spriteType > FIVE && spriteType > bestType)
					{
						bestType = spriteType;
						a = i * _Y + j + 1;
						b = rightSprite->x * _Y + rightSprite->y + 1;
						canGenerateProp = true;
					}
				}
				rightSprite->type = sprites[i][j].type;
				sprites[i][j].type = tempType;
			}
		}
	}
}

void Game::findPropEliminate(SpriteSet& spriteSet,SpriteType type)
{
	bool prop = true;
	while (prop)
	{
		//特殊元素消除
		SpriteSet::iterator it;
		for (it = spriteSet.begin(); it != spriteSet.end(); it++)
		{
			Sprite* sprite = *it;
			if (sprite->type > FIVE)
			{
				switch (sprite->type)
				{
				case ALL:
					for (int i = 0; i < _X; i++)
					{
						for (int j = 0; j < _Y; j++)
						{
							if (sprites[i][j].type == type)
							{
								spriteSet.insert(&sprites[i][j]);
							}
						}
					}
					break;
				case ALL_X:
					for (int j = 0; j < _Y; j++)
					{
						spriteSet.insert(&sprites[sprite->x][j]);
					}
					break;
				case ALL_Y:
					for (int i = 0; i < _X; i++)
					{
						spriteSet.insert(&sprites[i][sprite->y]);
					}
					break;
				case BOOM:
				{
					for (int i = max(0, sprite->x - 1); i <= min(_X - 1, sprite->x + 1); i++)
					{
						for (int j = max(0, sprite->y - 1); j <= min(_Y - 1, sprite->y + 1); j++)
						{
							spriteSet.insert(&sprites[i][j]);
						}
					}
					Sprite* upSprite = getUpSprite(sprite->x, sprite->y);
					Sprite* downSprite = getDownSprite(sprite->x, sprite->y);
					Sprite* leftSprite = getLeftSprite(sprite->x, sprite->y);
					Sprite* rightSprite = getRightSprite(sprite->x, sprite->y);
					if (upSprite->type != ZERO)
					{
						upSprite = getUpSprite(upSprite->x, upSprite->y);
						if (upSprite->type != ZERO)
						{
							spriteSet.insert(upSprite);
						}
					}
					if (downSprite->type != ZERO)
					{
						downSprite = getDownSprite(downSprite->x, downSprite->y);
						if (downSprite->type != ZERO)
						{
							spriteSet.insert(downSprite);
						}
					}
					if (leftSprite->type != ZERO)
					{
						leftSprite = getLeftSprite(leftSprite->x, leftSprite->y);
						if (leftSprite->type != ZERO)
						{
							spriteSet.insert(leftSprite);
						}
					}
					if (rightSprite->type != ZERO)
					{
						rightSprite = getRightSprite(rightSprite->x, rightSprite->y);
						if (rightSprite->type != ZERO)
						{
							spriteSet.insert(rightSprite);
						}
					}
					break;
				}
				case PLANE:
				{
					Sprite* upSprite = getUpSprite(sprite->x, sprite->y);
					Sprite* downSprite = getDownSprite(sprite->x, sprite->y);
					Sprite* leftSprite = getLeftSprite(sprite->x, sprite->y);
					Sprite* rightSprite = getRightSprite(sprite->x, sprite->y);
					if (upSprite->type != ZERO)
					{
						spriteSet.insert(upSprite);
					}
					if (downSprite->type != ZERO)
					{
						spriteSet.insert(downSprite);
					}
					if (leftSprite->type != ZERO)
					{
						spriteSet.insert(leftSprite);
					}
					if (rightSprite->type != ZERO)
					{
						spriteSet.insert(rightSprite);
					}
					break;
				}
				default:
					break;
				}
				sprite->type = ZERO;
			}

		}
		prop = false;
		for (it = spriteSet.begin(); it != spriteSet.end(); it++)
		{
			Sprite* sprite = *it;
			if (sprite->type > FIVE)
			{
				prop = true;
			}
		}
	}
}

void Game::checknoEliminate() 
{
	if (noEliminate())
	{
		cout << "没有可交换的元素了" << endl;
		do
		{
			for (int i = 0; i < _X; i++)
			{
				for (int j = 0; j < _Y; j++)
				{
					do
					{
						sprites[i][j].type = randomType();
					} while (canEliminateX(i, j) || canEliminateY(i, j) || canEliminateXY(i, j));
				}
			}
		} while (noEliminate());
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

SpriteType Game::randomWightType()
{
	map<SpriteType, int> spriteTypeCount;
	spriteTypeCount[ONE] = 0;
	spriteTypeCount[TWO] = 0;
	spriteTypeCount[THREE] = 0;
	spriteTypeCount[FOUR] = 0;
	spriteTypeCount[FIVE] = 0;
	int weight = 0;
	for (int i = 0; i < _X; i++)
	{
		for (int j = 0; j < _Y; j++)
		{
			SpriteType type = sprites[i][j].type;
			if (type > ZERO && type <= FIVE)
			{
				if (spriteTypeCount.find(type) == spriteTypeCount.end())
				{
					spriteTypeCount[type] = 1;
				}
				else 
				{
					spriteTypeCount[type] = spriteTypeCount[type] + 1;
				}
				weight++;
			}
		}
	}
	if (weight == 0)
	{
		return randomType();
	}
	weight = _X * _Y;
	int maxWeight = 0;
	map<SpriteType, int>::iterator it;
	for (it = spriteTypeCount.begin(); it != spriteTypeCount.end(); it++)
	{
		it->second = weight - it->second;
		maxWeight += it->second;
	}
	int sumWeight = 0;
	int number = rand() % maxWeight;
	for (it = spriteTypeCount.begin(); it != spriteTypeCount.end(); it++)
	{
		int addWight = it->second;
		if (sumWeight <= number && number < (sumWeight += addWight))
		{
			return it->first;
		}
	}
	return randomType();
}

void printColor(SpriteType type) {
	switch (type)
	{
	case ZERO:
		break;
	case ONE:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		break;
	case TWO:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		break;
	case THREE:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		break;
	case FOUR:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		break;
	case FIVE:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
		break;
	case ALL:
		break;
	case ALL_X:
		break;
	case ALL_Y:
		break;
	case BOOM:
		break;
	case PLANE:
		break;
	default:
		break;
	}
}

void Game::print() const
{
	cout << endl << "当前分数: " << score << endl;
	for (int i=0;i < _X;i++)
	{
		cout << endl;
		for (int j=0;j < _Y;j++)
		{
			printColor(sprites[i][j].type);
			switch (sprites[i][j].type)
			{
			case ALL:
				cout << " A";
				break;
			case BOOM:
				cout << " B";
				break;
			case ALL_X:
				cout << " X";
				break;
			case ALL_Y:
				cout << " Y";
				break;
			case PLANE:
				cout << " P";
				break;
			default:
				cout << " " << sprites[i][j].type;
				break;
			}
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED |
				FOREGROUND_GREEN | FOREGROUND_BLUE);
		}
	}
	cout << endl;
}

void Game::printHelpInfo()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	cout << "***********************************帮助说明***********************************" << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
	cout << "1到5代表5种基础元素，可横向、纵向、田字消除，每消除一个元素获得一分" << endl;
	cout << "特殊元素：" << endl;
	cout << "A：相同元素全消" << endl;
	cout << "B：炸弹，消3x3田字区域" << endl;
	cout << "X：横向元素全消" << endl;
	cout << "Y：纵向元素全消" << endl;
	cout << "P：飞机，消十字星区域" << endl;
	cout << "0：已消除的元素" << endl;
	cout << "操作方式：" << endl;
	cout << "这是个" << _X << "行" << _Y << "列的矩阵" << endl;
	cout << "一个整数代表一个坐标，比如13代表第" << (13 / _Y + 1) << "行,第" << (13 % _Y + 1) << "列的元素" << endl;
	cout << "依次输入两个整数，代表需要交换的两个元素，两个坐标相同代表双击" << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	cout << "***********************************开始游戏***********************************" << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void Game::printPropStatistics()
{
	int all = propStatistics.find(ALL) == propStatistics.end() ? 0 : propStatistics[ALL];
	int boom = propStatistics.find(BOOM) == propStatistics.end() ? 0 : propStatistics[BOOM];
	int all_x = propStatistics.find(ALL_X) == propStatistics.end() ? 0 : propStatistics[ALL_X];
	int all_y = propStatistics.find(ALL_Y) == propStatistics.end() ? 0 : propStatistics[ALL_Y];
	int plane = propStatistics.find(PLANE) == propStatistics.end() ? 0 : propStatistics[PLANE];
	cout << "生成道具统计：" << endl;
	cout << "A：" << all << endl;
	cout << "B：" << boom << endl;
	cout << "X：" << all_x << endl;
	cout << "Y：" << all_y << endl;
	cout << "P：" << plane << endl;
}