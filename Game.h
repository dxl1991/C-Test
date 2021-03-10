
#include <set>
#include "Sprite.h"
const int _X = 10;
const int _Y = 7;
class Game
{
public:
	Game();
	~Game();
	bool swapSprite(int a,int b);
	void checknoEliminate();
	SpriteType randomType();
    Sprite* NULL_SPRITE = new Sprite();
private:
	int score;
	Sprite sprites[_X][_Y];
	void print();
	bool canEliminateY(int x, int y);
	bool canEliminateX(int x, int y);
	bool canEliminateXY(int x, int y);
	Sprite* getUpSprite(int x, int y);
	Sprite* getDownSprite(int x, int y);
	Sprite* getLeftSprite(int x, int y);
	Sprite* getRightSprite(int x, int y);
	void findEliminateSprite(int x, int y, std::set<Sprite*>& spriteSet, bool directX);
	void findEliminateSpriteXY(int x, int y, std::set<Sprite*>& spriteSet);
	void moveSprite(std::set<Sprite*>& spriteSet);
	bool EliminateSprite(int x, int y, bool useProp,SpriteType type);
	void generateProp(std::set<Sprite*>& spriteSet, int originX, int originY);
	void EliminateSprite(Sprite* sprite1, Sprite* sprite2);
	bool noEliminate();
	void findPropEliminate(std::set<Sprite*>& spriteSet, SpriteType type);
};

