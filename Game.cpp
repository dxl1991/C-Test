#include <iostream>
#include <time.h>
#include <algorithm>
#include <map>
#include "Game.h"
#include "windows.h"
using namespace std;

Game::Game()
{
	srand((int)time(0));
	this->score = 0;
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
	print();
}
Game::~Game() 
{
	delete NULL_SPRITE;
	NULL_SPRITE = NULL;
}

//消除
bool Game::EliminateSprite(int x,int y,bool useProp,SpriteType type)
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
	set<Sprite*> spriteSetTemp = spriteSet;
	findPropEliminate(&spriteSet,type);
	set<Sprite*>::iterator it;
	for (it = spriteSet.begin(); it != spriteSet.end(); it++)
	{
		Sprite* sprite = *it;
		sprite->type = ZERO;
	}
	this->score += spriteSet.size();
	generateProp(&spriteSetTemp, x, y);
	spriteSet.clear();
	print();
	moveSprite(&spriteSet);
	print();
	//判断是否可以再消除
	for (it = spriteSet.begin(); it != spriteSet.end(); it++)
	{
		Sprite* sprite = *it;
		EliminateSprite(sprite->x, sprite->y, false, randomType());
	}
	return true;
}
//产生特殊元素
void Game::generateProp(set<Sprite*>* spriteSet,int originX,int originY)
{
	map<int, int> xCount, yCount;
	set<Sprite*>::iterator it;
	for (it = spriteSet->begin(); it != spriteSet->end(); it++)
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
	if (xCount.size() == 2 && yCount.size() == 2 && (spriteSet->size() == 4 || spriteSet->size() == 5))
	{
		sprites[originX][originY].type = PLANE;
		return;
	}
	bool three = false;
	map<int, int>::iterator iter;
	for (iter = xCount.begin(); iter != xCount.end(); iter++)
	{
		if (iter->second >= 5)
		{
			sprites[originX][originY].type = ALL;
			return;
		}
		if (iter->second == 4)
		{
			sprites[originX][originY].type = ALL_Y;
			three = true;
		}
		if (iter->second == 3)
		{
			three = true;
		}

	}
	for (iter = yCount.begin(); iter != yCount.end(); iter++)
	{
		if (iter->second >= 5)
		{
			sprites[originX][originY].type = ALL;
			return;
		}
		if (iter->second == 4)
		{
			sprites[originX][originY].type = ALL_X;
		}
		if (iter->second == 3 && three)
		{
			sprites[originX][originY].type = BOOM;
		}
	}
}
//特殊元素交换
void Game::EliminateSprite(Sprite* sprite1, Sprite* sprite2)
{
	set<Sprite*> spriteSet;
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
		if (sprite1->type ==ALL)
		{
			sprite1->type = ZERO;
		}
		if (sprite2->type == ALL)
		{
			sprite2->type = ZERO;
		}
		spriteSet.insert(sprite1);
		spriteSet.insert(sprite2);
		SpriteType type = randomType();
		for (int i = 0; i < _X; i++)
		{
			for (int j = 0; j < _Y; j++)
			{
				sprites[i][j].type = sprite1->type == ALL ? sprite2->type : sprite1->type;
				spriteSet.insert(&sprites[i][j]);
			}
		}
	}
	if ((sprite1->type == BOOM && (sprite2->type == ALL_X || sprite2->type == ALL_Y)) || (sprite2->type == BOOM && (sprite1->type == ALL_X || sprite1->type == ALL_Y)))
	{
		int min_x, max_x, min_y, max_y;
		min_x = min(sprite1->x, sprite2->x);
		max_x = min(min_x + 3, _X);
		min_y = min(sprite1->y, sprite2->y);
		max_y = min(min_y + 3, _Y);
		for (int i = min_x; i < max_x; i++)
		{
			for (int j = 0; j < _Y; j++)
			{
				spriteSet.insert(&sprites[i][j]);
			}
		}
		for (int i = 0; i < _X; i++)
		{
			for (int j = min_y; j < max_y; j++)
			{
				spriteSet.insert(&sprites[i][j]);
			}
		}
		sprite1->type = ZERO;
		sprite2->type = ZERO;
	}
	if (sprite1->type == BOOM && sprite2->type == BOOM)
	{
		int min_x, max_x, min_y, max_y;
		min_x = min(sprite1->x, sprite2->x);
		min_x = max(min_x - 2, 0);
		max_x = max(sprite1->x, sprite2->x);
		max_x = min(max_x + 2, _X);
		min_y = min(sprite1->y, sprite2->y);
		min_y = max(min_y - 2, 0);
		max_y = max(sprite1->y, sprite2->y);
		max_y = min(max_y + 2, _Y);
		for (int i = min_x; i < max_x; i++)
		{
			for (int j = min_y; j < max_y; j++)
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
			spriteSet.insert(&sprites[i][sprite1->y]);
		}

		for (int j = 0; j < _Y; j++)
		{
			spriteSet.insert(&sprites[sprite1->x][j]);
		}
		sprite1->type = ZERO;
		sprite2->type = ZERO;
	}
	findPropEliminate(&spriteSet, randomType());
	set<Sprite*>::iterator it;
	for (it = spriteSet.begin(); it != spriteSet.end(); it++)
	{
		Sprite* sprite = *it;
		sprite->type = ZERO;
	}
	this->score += spriteSet.size();
	spriteSet.clear();
	print();
	moveSprite(&spriteSet);
	print();
	//判断是否可以再消除
	for (it = spriteSet.begin(); it != spriteSet.end(); it++)
	{
		Sprite* sprite = *it;
		EliminateSprite(sprite->x, sprite->y, false, randomType());
	}
}

//坐标交换
bool Game::swapSprite(int a, int b)
{

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
			return true;
		}

		cout << "不能输入相同坐标";
		return false;
	}

	if ((x1 == x2 && abs(y1 - y2) == 1) || (y1 == y2 && abs(x1 - x2) == 1))
	{
		SpriteType temp = sprites[x1][y1].type;
		sprites[x1][y1].type = sprites[x2][y2].type;
		sprites[x2][y2].type = temp;

		//两个特殊元素交换
		if (sprites[x1][y1].type > FIVE && sprites[x2][y2].type > FIVE)
		{
			EliminateSprite(&sprites[x1][y1], &sprites[x2][y2]);
			return true;
		}

		bool flag1, flag2;
		if (sprites[x1][y1].type > FIVE || x1 < x2)
		{
			flag1 = EliminateSprite(x1, y1, true, sprites[x2][y2].type);
			flag2 = EliminateSprite(x2, y2, true, sprites[x1][y1].type);
		}
		else
		{
			flag1 = EliminateSprite(x2, y2, true, sprites[x1][y1].type);
			flag2 = EliminateSprite(x1, y1, true, sprites[x2][y2].type);
		}

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
				spriteSet->insert(self);
				spriteSet->insert(upSprite);
				spriteSet->insert(leftSprite);
				spriteSet->insert(sprite);
			}
		}
		if (rightSprite->type == self->type)
		{
			Sprite* sprite = getUpSprite(rightSprite->x, rightSprite->y);
			if (sprite->type == self->type)
			{
				spriteSet->insert(self);
				spriteSet->insert(upSprite);
				spriteSet->insert(rightSprite);
				spriteSet->insert(sprite);
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
				spriteSet->insert(self);
				spriteSet->insert(downSprite);
				spriteSet->insert(leftSprite);
				spriteSet->insert(sprite);
			}
		}
		if (rightSprite->type == self->type)
		{
			Sprite* sprite = getDownSprite(rightSprite->x, rightSprite->y);
			if (sprite->type == self->type)
			{
				spriteSet->insert(self);
				spriteSet->insert(downSprite);
				spriteSet->insert(rightSprite);
				spriteSet->insert(sprite);
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

void Game::findPropEliminate(std::set<Sprite*>* spriteSet,SpriteType type)
{
	bool prop = true;
	while (prop)
	{
		//特殊元素消除
		set<Sprite*>::iterator it;
		for (it = spriteSet->begin(); it != spriteSet->end(); it++)
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
								spriteSet->insert(&sprites[i][j]);
							}
						}
					}
					break;
				case ALL_X:
					for (int j = 0; j < _Y; j++)
					{
						spriteSet->insert(&sprites[sprite->x][j]);
					}
					break;
				case ALL_Y:
					for (int i = 0; i < _X; i++)
					{
						spriteSet->insert(&sprites[i][sprite->y]);
					}
					break;
				case BOOM:
					for (int i = max(0, sprite->x - 1); i <= min(_X - 1, sprite->x + 1); i++)
					{
						for (int j = max(0, sprite->y - 1); j <= min(_Y - 1, sprite->y + 1); j++)
						{
							spriteSet->insert(&sprites[i][j]);
						}
					}
					break;
				case PLANE:
				{
					Sprite* upSprite = getUpSprite(sprite->x, sprite->y);
					Sprite* downSprite = getDownSprite(sprite->x, sprite->y);
					Sprite* leftSprite = getLeftSprite(sprite->x, sprite->y);
					Sprite* rightSprite = getRightSprite(sprite->x, sprite->y);
					if (upSprite->type != ZERO)
					{
						spriteSet->insert(upSprite);
					}
					if (downSprite->type != ZERO)
					{
						spriteSet->insert(downSprite);
					}
					if (leftSprite->type != ZERO)
					{
						spriteSet->insert(leftSprite);
					}
					if (rightSprite->type != ZERO)
					{
						spriteSet->insert(rightSprite);
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
		for (it = spriteSet->begin(); it != spriteSet->end(); it++)
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
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED |
			FOREGROUND_GREEN);
		break;
	case FIVE:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED |
			FOREGROUND_BLUE);
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

void Game::print()
{
	cout << endl << "score: " << score << endl;
	for (int i=0;i < _X;i++)
	{
		cout << endl;
		for (int j=0;j < _Y;j++)
		{
			printColor(sprites[i][j].type);
			cout << " " << sprites[i][j].type;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED |
				FOREGROUND_GREEN | FOREGROUND_BLUE);
		}
	}
	cout << endl;
}