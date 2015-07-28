#include "PolygonLayer.h"

USING_NS_CC;

PolygonLayer::PolygonLayer(void)
{
}

PolygonLayer::~PolygonLayer(void)
{
}

bool PolygonLayer::init()
{
	if (!Layer::init())
    {
        return false;
	}

	DrawNode *drawnode = DrawNode::create();
	drawnode->setTag(1);
	this->addChild(drawnode);

	return true;
}

void PolygonLayer::addVertex(const cocos2d::Vec2& vertex)
{
	/*check whether the new vertex creates a line that intersect with
	any of the already existing lines*/
	if (vertextIntersectWithCurrentVertices(vertex))
	{
		_currentVertices.push_back(vertex);
		DrawNode *tmp = DrawNode::create();
		this->addChild(tmp);

		tmp->drawPolygon(&_currentVertices[0],
						_currentVertices.size(),
						Color4F(1,1,1,0.1f),
						4,
						Color4F(1,0,0,1.0f));


		tmp->runAction(Sequence::create(
			ScaleBy::create(1.0f,1.0f),
			RemoveSelf::create(true),
			nullptr));
		 
		clear();
		return;
	}

	this->_currentVertices.push_back(vertex);
	auto  drawnode = ((DrawNode*)getChildByTag(1));
	drawnode->clear();
	drawnode->drawPoints(&_currentVertices[0],
		_currentVertices.size(),
		15,
		Color4F(0,1,0,0.5f));

	for(unsigned long i = 0 ; i < _currentVertices.size() - 1 && _currentVertices.size() >= 2 ; ++i)
	{
		drawnode->drawLine(
			_currentVertices[i],
			_currentVertices[i+1],
			Color4F(1,1,1,0.5f));
	}
}

void PolygonLayer::fadeFinished(Node* node) {
	removeChild(node);
}

void PolygonLayer::clear()
{
	((DrawNode*)getChildByTag(1))->clear();
	_currentVertices.clear();
}

bool PolygonLayer::vertextIntersectWithCurrentVertices(const cocos2d::Vec2& vec) const
{
	size_t currentVerticesSize = _currentVertices.size();
	if (currentVerticesSize <= 2)
	{
		return false;
	}

	auto lastVertext = _currentVertices.back();

	for (size_t i = 0 ; i < currentVerticesSize - 2 ; i++)
	{
		auto lineVertex1 = _currentVertices[i];
		auto lineVertex2 = _currentVertices[i+1];
		if (Vec2::isSegmentIntersect(vec, lastVertext, lineVertex1, lineVertex2))
		{
			return true;
		}
	}

	return false;
}