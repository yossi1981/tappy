#ifndef __GameScene_LAYER_H__
#define __GameScene_LAYER_H__

#include "cocos2d.h"
#include <vector>
#include <map>

class GameScene : public cocos2d::Scene
{
public:
	virtual bool init();

	void addEventListener();

	void addBackground(cocos2d::Size &visibleSize);

	void createPolygonLayer();

	void createHudLayer();

	CREATE_FUNC(GameScene);

private:
	void update(float dt);

	int _score;
	float _multiplier;
	float _totalTimePassed;

	unsigned long _rows;
	unsigned long _cols;

	float _colWidth;
	float _rowWidth;
	
	cocos2d::Vec2 _gameAreaOffset;
	/**TODO optimization : change it to a hash or BST*/
	typedef std::vector<cocos2d::Vec2> PointsCollection_t;
	typedef unsigned long LocationDim_t;

	PointsCollection_t _occupiedLocations;
	PointsCollection_t _freeLocations;

	typedef std::map<int, int> BaloonsPerColor;
	BaloonsPerColor _baloonsPerColor;



	static cocos2d::TTFConfig createScoreLabelConfig();

};

#endif