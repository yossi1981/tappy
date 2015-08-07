#ifndef __GameScene_LAYER_H__
#define __GameScene_LAYER_H__

#include "cocos2d.h"
#include <vector>
#include <map>
#include "Defs.h"


class GameScene : public cocos2d::Scene
{
public:
	virtual bool init();
	CREATE_FUNC(GameScene);

private:
	int _score;
	float _multiplier;
	float _totalTimePassed;

	unsigned long _rows;
	unsigned long _cols;

	float _colWidth;
	float _rowWidth;
	
	cocos2d::Vec2 _gameAreaOffset;

	ColorSequenceState_t _colorSequenceState_t;

	/**TODO optimization : change it to a hash or BST*/
	typedef std::vector<cocos2d::Vec2> PointsCollection_t;
	PointsCollection_t _occupiedLocations;
	PointsCollection_t _freeLocations;

	typedef std::map<int, int> BaloonsPerColor;
	BaloonsPerColor _baloonsPerColor;

	void update(float dt);
	void InitDataMembers();
	void PlayBackgroundMusic();
	void addEventListener();
	void AddExplostion(const cocos2d::Vec2& position);
	void addBackground();
	void createPolygonLayer();
	void createHudLayer();
	void resetColorSequenceState();

	static cocos2d::TTFConfig createScoreLabelConfig();
};

#endif