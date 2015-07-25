#pragma once
#include <cocos2d.h>


class PolygonLayer : public cocos2d::Layer
{
public:
	CREATE_FUNC(PolygonLayer);
	PolygonLayer(void);
	virtual ~PolygonLayer(void);

	virtual bool init();
	void addVertex(const cocos2d::Vec2& vertex);
	void clear();

protected:
	typedef std::vector<cocos2d::Vec2> PointsCollection_t;
	PointsCollection_t _currentVertices;

private:
	bool vertextIntersectWithCurrentVertices(const cocos2d::Vec2& vec) const;
	void fadeFinished(Node* node);
};

