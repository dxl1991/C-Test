
#include <set>
#include "Sprite.h"
typedef std::set<Sprite*> SpriteSet; //类型别名

class Game
{
public:
	Game();
	Game(int x,int y);
	~Game();
	bool swapSprite(int a,int b);
	void checknoEliminate();
	void findSuggest(int& a,int& b);
	void printPropStatistics();
	SpriteType randomType();
	SpriteType randomWightType();
    Sprite* NULL_SPRITE = new Sprite();
	std::map<SpriteType, int> propStatistics;
	int score;
	int _X;
	int _Y;
private:
	Sprite** sprites;
	void print();
	void printHelpInfo();
	bool canEliminateY(int x, int y);
	bool canEliminateX(int x, int y);
	bool canEliminateXY(int x, int y);
	Sprite* getUpSprite(int x, int y);
	Sprite* getDownSprite(int x, int y);
	Sprite* getLeftSprite(int x, int y);
	Sprite* getRightSprite(int x, int y);
	void findEliminateSpriteAll(int x, int y, SpriteSet& spriteSet);
	void findEliminateSprite(int x, int y, SpriteSet& spriteSet, bool directX);
	void findEliminateSpriteXY(int x, int y, SpriteSet& spriteSet);
	void moveSprite(SpriteSet& spriteSet);
	bool EliminateSprite(int x, int y, bool useProp,SpriteType type);
	SpriteType generateProp(SpriteSet& spriteSet);
	void EliminateSprite1(Sprite* sprite1, Sprite* sprite2);
	void EliminateSprite2(Sprite* sprite1, Sprite* sprite2);
	bool noEliminate();
	void findPropEliminate(SpriteSet& spriteSet, SpriteType type);
};

