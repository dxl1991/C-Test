
#include <set>
#include <map>
#include "Sprite.h"
#include "CSRandom.h"

typedef std::set<Sprite*> SpriteSet; //类型别名
const bool forbidPlane = true;

class Game
{
public:
	Game();
	Game(int x, int y);
	~Game();
	bool swapSprite(int a, int b);
	void checknoEliminate();
	void findSuggest(int& a, int& b);
	void printPropStatistics();
	SpriteType randomType();
	SpriteType randomWightType();
	Sprite* NULL_SPRITE = new Sprite();
	std::map<SpriteType, int> propStatistics;
	int score;
	int seed;
	int _X;
	int _Y;
private:
	CSRandom* csRandom;
	Sprite** sprites;
	void print() const; //方法后面跟const，此方法不能修改成员变量的值
	void printHelpInfo();
	bool canEliminateY(int x, int y);
	bool canEliminateX(int x, int y);
	bool canEliminateXY(int x, int y);
	Sprite* getUpSprite(int x, int y);
	Sprite* getDownSprite(int x, int y);
	Sprite* getLeftSprite(int x, int y);
	Sprite* getRightSprite(int x, int y);
	void findEliminateSpriteAll(int x, int y, SpriteSet& spriteSet, bool recursion);
	void findEliminateSprite(int x, int y, SpriteSet& spriteSet, bool directX, int direct);
	void findEliminateSpriteXY(int x, int y, SpriteSet& spriteSet);
	void moveSprite();
	bool EliminateSprite(int x, int y, bool useProp, SpriteType type);
	bool EliminateSprite(SpriteSet& spriteSet);
	SpriteType generateProp(SpriteSet& spriteSet);
	void EliminateSprite1(Sprite* sprite1, Sprite* sprite2);
	void EliminateSprite2(Sprite* sprite1, Sprite* sprite2);
	bool EliminateSprite3(Sprite* sprite1, Sprite* sprite2);
	bool noEliminate();
	void findPropEliminate(SpriteSet& spriteSet, SpriteType type);
	SpriteType tryEliminate(int x, int y);
};

template <typename T>
T** new_Array2D(int row, int col)
{
	int size = sizeof(T);
	int point_size = sizeof(T*);
	//先申请内存，其中sizeof(T*) * row表示存放row个行指针
	T** arr = (T**)malloc(size * row * col);
	if (arr != NULL)
	{
		T* head = (T*)((int)arr);
		for (int i = 0; i < row; ++i)
		{
			arr[i] = (T*)((int)head + i * col * size);
			//for (int j = 0; j < col; ++j)
				//new (&arr[i][j]) T;
		}
	}
	return (T**)arr;
}
//释放二维数组
template <typename T>
void delete_Array2D(T** arr, int row, int col)
{
	for (int i = 0; i < row; ++i)
		for (int j = 0; j < col; ++j)
			arr[i][j].~T();
	if (arr != NULL)
		free((void**)arr);
}

