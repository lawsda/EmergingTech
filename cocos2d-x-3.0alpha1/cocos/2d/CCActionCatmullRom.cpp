/*
 * Copyright (c) 2010-2012 cocos2d-x.org
 * cocos2d for iPhone: http://www.cocos2d-iphone.org
 *
 * Copyright (c) 2008 Radu Gruian
 *
 * Copyright (c) 2011 Vit Valentin
 *
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 *
 * Original code by Radu Gruian: http://www.codeproject.com/Articles/30838/Overhauser-Catmull-Rom-Splines-for-Camera-Animatio.So
 *
 * Adapted to cocos2d-x by Vit Valentin
 *
 * Adapted from cocos2d-x to cocos2d-iphone by Ricardo Quesada
 */
#include "ccMacros.h"
#include "CCActionCatmullRom.h"

using namespace std;

NS_CC_BEGIN;

/*
 *  Implementation of PointArray
 */

PointArray* PointArray::create(unsigned int capacity)
{
    PointArray* pointArray = new PointArray();
    if (pointArray)
    {
        if (pointArray->initWithCapacity(capacity))
        {
            pointArray->autorelease();
        }
        else 
        {
            delete pointArray;
            pointArray = nullptr;
        }
    }

    return pointArray;
}


bool PointArray::initWithCapacity(unsigned int capacity)
{
    _controlPoints = new vector<Point*>();
    
    return true;
}

PointArray* PointArray::clone() const
{
    vector<Point*> *newArray = new vector<Point*>();
    vector<Point*>::iterator iter;
    for (iter = _controlPoints->begin(); iter != _controlPoints->end(); ++iter)
    {
        newArray->push_back(new Point((*iter)->x, (*iter)->y));
    }
    
    PointArray *points = new PointArray();
    points->initWithCapacity(10);
    points->setControlPoints(newArray);

    points->autorelease();
    return points;
}

PointArray::~PointArray()
{
    CCLOGINFO("deallocing PointArray: %p", this);

    vector<Point*>::iterator iter;
    for (iter = _controlPoints->begin(); iter != _controlPoints->end(); ++iter)
    {
        delete *iter;
    }
    delete _controlPoints;
}

PointArray::PointArray() :_controlPoints(nullptr){}

const std::vector<Point*>* PointArray::getControlPoints() const
{
    return _controlPoints;
}

void PointArray::setControlPoints(vector<Point*> *controlPoints)
{
    CCASSERT(controlPoints != nullptr, "control points should not be nullptr");
    
    // delete old points
    vector<Point*>::iterator iter;
    for (iter = _controlPoints->begin(); iter != _controlPoints->end(); ++iter)
    {
        delete *iter;
    }
    delete _controlPoints;
    
    _controlPoints = controlPoints;
}

void PointArray::addControlPoint(Point controlPoint)
{    
    _controlPoints->push_back(new Point(controlPoint.x, controlPoint.y));
}

void PointArray::insertControlPoint(Point &controlPoint, unsigned int index)
{
    Point *temp = new Point(controlPoint.x, controlPoint.y);
    _controlPoints->insert(_controlPoints->begin() + index, temp);
}

Point PointArray::getControlPointAtIndex(unsigned int index)
{
    index = MIN(_controlPoints->size()-1, MAX(index, 0));
    return *(_controlPoints->at(index));
}

void PointArray::replaceControlPoint(cocos2d::Point &controlPoint, unsigned int index)
{

    Point *temp = _controlPoints->at(index);
    temp->x = controlPoint.x;
    temp->y = controlPoint.y;
}

void PointArray::removeControlPointAtIndex(unsigned int index)
{
    vector<Point*>::iterator iter = _controlPoints->begin() + index;
    Point* pRemovedPoint = *iter;
    _controlPoints->erase(iter);
    delete pRemovedPoint;
}

unsigned int PointArray::count() const
{
    return _controlPoints->size();
}

PointArray* PointArray::reverse() const
{
    vector<Point*> *newArray = new vector<Point*>();
    vector<Point*>::reverse_iterator iter;
    Point *point = nullptr;
    for (iter = _controlPoints->rbegin(); iter != _controlPoints->rend(); ++iter)
    {
        point = *iter;
        newArray->push_back(new Point(point->x, point->y));
    }
    PointArray *config = PointArray::create(0);
    config->setControlPoints(newArray);
    
    return config;
}

void PointArray::reverseInline()
{
    unsigned long l = _controlPoints->size();
    Point *p1 = nullptr;
    Point *p2 = nullptr;
    int x, y;
    for (unsigned int i = 0; i < l/2; ++i)
    {
        p1 = _controlPoints->at(i);
        p2 = _controlPoints->at(l-i-1);
        
        x = p1->x;
        y = p1->y;
        
        p1->x = p2->x;
        p1->y = p2->y;
        
        p2->x = x;
        p2->y = y;
    }
}

// CatmullRom Spline formula:
Point ccCardinalSplineAt(Point &p0, Point &p1, Point &p2, Point &p3, float tension, float t)
{
    float t2 = t * t;
    float t3 = t2 * t;
    
	/*
	 * Formula: s(-ttt + 2tt - t)P1 + s(-ttt + tt)P2 + (2ttt - 3tt + 1)P2 + s(ttt - 2tt + t)P3 + (-2ttt + 3tt)P3 + s(ttt - tt)P4
	 */
    float s = (1 - tension) / 2;
	
    float b1 = s * ((-t3 + (2 * t2)) - t);                      // s(-t3 + 2 t2 - t)P1
    float b2 = s * (-t3 + t2) + (2 * t3 - 3 * t2 + 1);          // s(-t3 + t2)P2 + (2 t3 - 3 t2 + 1)P2
    float b3 = s * (t3 - 2 * t2 + t) + (-2 * t3 + 3 * t2);      // s(t3 - 2 t2 + t)P3 + (-2 t3 + 3 t2)P3
    float b4 = s * (t3 - t2);                                   // s(t3 - t2)P4
    
    float x = (p0.x*b1 + p1.x*b2 + p2.x*b3 + p3.x*b4);
    float y = (p0.y*b1 + p1.y*b2 + p2.y*b3 + p3.y*b4);
	
	return Point(x,y);
}

/* Implementation of CardinalSplineTo
 */

CardinalSplineTo* CardinalSplineTo::create(float duration, cocos2d::PointArray *points, float tension)
{
    CardinalSplineTo *ret = new CardinalSplineTo();
    if (ret)
    {
        if (ret->initWithDuration(duration, points, tension))
        {
            ret->autorelease();
        }
        else 
        {
            CC_SAFE_RELEASE_NULL(ret);
        }
    }

    return ret;
}

bool CardinalSplineTo::initWithDuration(float duration, cocos2d::PointArray *points, float tension)
{
    CCASSERT(points->count() > 0, "Invalid configuration. It must at least have one control point");

    if (ActionInterval::initWithDuration(duration))
    {
        this->setPoints(points);
        this->_tension = tension;
        
        return true;
    }
    
    return false;
}

CardinalSplineTo::~CardinalSplineTo()
{
    CC_SAFE_RELEASE_NULL(_points);
}

CardinalSplineTo::CardinalSplineTo()
: _points(nullptr)
, _deltaT(0.f)
, _tension(0.f)
{
}

void CardinalSplineTo::startWithTarget(cocos2d::Node *target)
{
    ActionInterval::startWithTarget(target);
	
//    _deltaT = (float) 1 / _points->count();
    
    // Issue #1441
    _deltaT = (float) 1 / (_points->count() - 1);

    _previousPosition = target->getPosition();
    _accumulatedDiff = Point::ZERO;
}

CardinalSplineTo* CardinalSplineTo::clone() const
{
	// no copy constructor
	auto a = new CardinalSplineTo();
	a->initWithDuration(this->_duration, this->_points->clone(), this->_tension);
	a->autorelease();
	return a;
}

void CardinalSplineTo::update(float time)
{
    unsigned int p;
    float lt;
	
	// eg.
	// p..p..p..p..p..p..p
	// 1..2..3..4..5..6..7
	// want p to be 1, 2, 3, 4, 5, 6
    if (time == 1)
    {
        p = _points->count() - 1;
        lt = 1;
    }
    else 
    {
        p = time / _deltaT;
        lt = (time - _deltaT * (float)p) / _deltaT;
    }
    
	// Interpolate    
    Point pp0 = _points->getControlPointAtIndex(p-1);
    Point pp1 = _points->getControlPointAtIndex(p+0);
    Point pp2 = _points->getControlPointAtIndex(p+1);
    Point pp3 = _points->getControlPointAtIndex(p+2);
	
    Point newPos = ccCardinalSplineAt(pp0, pp1, pp2, pp3, _tension, lt);
	
#if CC_ENABLE_STACKABLE_ACTIONS
    // Support for stacked actions
    Node *node = _target;
    Point diff = node->getPosition() - _previousPosition;
    if( diff.x !=0 || diff.y != 0 ) {
        _accumulatedDiff = _accumulatedDiff + diff;
        newPos = newPos + _accumulatedDiff;
    }
#endif
    
    this->updatePosition(newPos);
}

void CardinalSplineTo::updatePosition(cocos2d::Point &newPos)
{
    _target->setPosition(newPos);
    _previousPosition = newPos;
}

CardinalSplineTo* CardinalSplineTo::reverse() const
{
    PointArray *pReverse = _points->reverse();
    
    return CardinalSplineTo::create(_duration, pReverse, _tension);
}

/* CardinalSplineBy
 */

CardinalSplineBy* CardinalSplineBy::create(float duration, cocos2d::PointArray *points, float tension)
{
    CardinalSplineBy *ret = new CardinalSplineBy();
    if (ret)
    {
        if (ret->initWithDuration(duration, points, tension))
        {
            ret->autorelease();
        }
        else 
        {
            CC_SAFE_RELEASE_NULL(ret);
        }
    }

    return ret;
}

CardinalSplineBy::CardinalSplineBy() : _startPosition(0,0)
{
}

void CardinalSplineBy::updatePosition(cocos2d::Point &newPos)
{
    Point p = newPos + _startPosition;
    _target->setPosition(p);
    _previousPosition = p;
}

CardinalSplineBy* CardinalSplineBy::reverse() const
{
    PointArray *copyConfig = _points->clone();
	
	//
	// convert "absolutes" to "diffs"
	//
    Point p = copyConfig->getControlPointAtIndex(0);
    for (unsigned int i = 1; i < copyConfig->count(); ++i)
    {
        Point current = copyConfig->getControlPointAtIndex(i);
        Point diff = current - p;
        copyConfig->replaceControlPoint(diff, i);
        
        p = current;
    }
	
	
	// convert to "diffs" to "reverse absolute"
	
    PointArray *pReverse = copyConfig->reverse();
	
	// 1st element (which should be 0,0) should be here too
    
    p = pReverse->getControlPointAtIndex(pReverse->count()-1);
    pReverse->removeControlPointAtIndex(pReverse->count()-1);
    
    p = -p;
    pReverse->insertControlPoint(p, 0);
    
    for (unsigned int i = 1; i < pReverse->count(); ++i)
    {
        Point current = pReverse->getControlPointAtIndex(i);
        current = -current;
        Point abs = current + p;
        pReverse->replaceControlPoint(abs, i);
        
        p = abs;
    }
	
    return CardinalSplineBy::create(_duration, pReverse, _tension);
}

void CardinalSplineBy::startWithTarget(cocos2d::Node *target)
{    
    CardinalSplineTo::startWithTarget(target);
    _startPosition = target->getPosition();
}

CardinalSplineBy* CardinalSplineBy::clone() const
{
	// no copy constructor
	auto a = new CardinalSplineBy();
	a->initWithDuration(this->_duration, this->_points->clone(), this->_tension);
	a->autorelease();
	return a;
}

/* CatmullRomTo
 */

CatmullRomTo* CatmullRomTo::create(float dt, cocos2d::PointArray *points)
{
    CatmullRomTo *ret = new CatmullRomTo();
    if (ret)
    {
        if (ret->initWithDuration(dt, points))
        {
            ret->autorelease();
        }
        else 
        {
            CC_SAFE_RELEASE_NULL(ret);
        }
    }

    return ret;
}

bool CatmullRomTo::initWithDuration(float dt, cocos2d::PointArray *points)
{
    if (CardinalSplineTo::initWithDuration(dt, points, 0.5f))
    {
        return true;
    }
    
    return false;
}

CatmullRomTo* CatmullRomTo::clone() const
{
	// no copy constructor
	auto a = new CatmullRomTo();
	a->initWithDuration(this->_duration, this->_points->clone());
	a->autorelease();
	return a;
}

CatmullRomTo* CatmullRomTo::reverse() const
{
    PointArray *pReverse = _points->reverse();
    return CatmullRomTo::create(_duration, pReverse);
}


/* CatmullRomBy
 */

CatmullRomBy* CatmullRomBy::create(float dt, cocos2d::PointArray *points)
{
    CatmullRomBy *ret = new CatmullRomBy();
    if (ret)
    {
        if (ret->initWithDuration(dt, points))
        {
            ret->autorelease();
        }
        else 
        {
            CC_SAFE_RELEASE_NULL(ret);
        }
    }

    return ret;
}

bool CatmullRomBy::initWithDuration(float dt, cocos2d::PointArray *points)
{
    if (CardinalSplineTo::initWithDuration(dt, points, 0.5f))
    {
        return true;
    }
    
    return false;
}

CatmullRomBy* CatmullRomBy::clone() const
{
	// no copy constructor	
	auto a = new CatmullRomBy();
	a->initWithDuration(this->_duration, this->_points->clone());
	a->autorelease();
	return a;
}

CatmullRomBy* CatmullRomBy::reverse() const
{
    PointArray *copyConfig = _points->clone();

	//
	// convert "absolutes" to "diffs"
	//
    Point p = copyConfig->getControlPointAtIndex(0);
    for (unsigned int i = 1; i < copyConfig->count(); ++i)
    {
        Point current = copyConfig->getControlPointAtIndex(i);
        Point diff = current - p;
        copyConfig->replaceControlPoint(diff, i);

        p = current;
    }


	// convert to "diffs" to "reverse absolute"

    PointArray *reverse = copyConfig->reverse();

	// 1st element (which should be 0,0) should be here too

    p = reverse->getControlPointAtIndex(reverse->count()-1);
    reverse->removeControlPointAtIndex(reverse->count()-1);

    p = -p;
    reverse->insertControlPoint(p, 0);

    for (unsigned int i = 1; i < reverse->count(); ++i)
    {
        Point current = reverse->getControlPointAtIndex(i);
        current = -current;
        Point abs = current + p;
        reverse->replaceControlPoint(abs, i);

        p = abs;
    }

    return CatmullRomBy::create(_duration, reverse);
}

NS_CC_END;

