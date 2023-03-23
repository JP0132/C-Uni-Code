#include "Geometry.h"

// ============ Shape class =================

//Shape::Shape() {} // REMOVE ME

Shape::Shape(int d) {
	if (d < 0) {
		throw std::invalid_argument("Shape depth cannot be negative");
	} else {
		depth = d;
	}
}

bool Shape::setDepth(int d) {
	if (d<0) {
		return false;
	} else {
		depth = d; //??
		return true;
	}
}

int Shape::getDepth() const {
	if (depth < 0) {
		return false;
	} else {
		return depth;
	}

	return -999; // dummy
}

int Shape::dim() const {
	return dimension; //(0,1 or 2)
}

void Shape::translate(float x, float y) {
	
}

void Shape::rotate() {
	// IMPLEMENT ME
}

void Shape::scale(float f) {
	// IMPLEMENT ME
}

bool Shape::contains(const Point& p) const {
	// IMPLEMENT ME
	return false; // dummy
}

// =============== Point class ================

Point::Point(float x, float y, int d=0) : Shape(d) {
	if (d < 0) {
		throw std::invalid_argument("Depth cannot be negative");
	} else {
		x_ = x;
		y_ = y;
		depth = d;
		dimension = 0;
	}
	
}

void Point::translate(float x, float y) {
	float newX = x_ + x;
	float newY = y_ + y;
	x_ = newX;
	y_ = newY;
}

bool Point::contains(const Point& p) const {
	if (x_ == p.getX() && y_ == p.getY()) {
		return true;
	} else {
		return false;
	}
}

void Point::rotate() {
}

void Point::scale(float f){
}

float Point::getX() const {
	return x_;
}

float Point::getY() const {
	return y_;
}

int Point::dim() const {
	return dimension; //(0,1 or 2)
}

// =========== LineSegment class ==============

LineSegment::LineSegment(const Point& p, const Point& q) : Shape(p.getDepth()) {
	float px = p.getX();
	float py = p.getY();
	float qx = q.getX();
	float qy = q.getY();

	if (p.getDepth() != q.getDepth()) {
		throw std::invalid_argument("Depth of line segment points must be the same");
	} else if (px == qx && py == qy) {
		throw std::invalid_argument("Both points cannot have the same coordinates");
	} else if (px != qx && py != qy) {
		throw std::invalid_argument("Line segment must be axis-aligned");
	} else {
		if (px != qx) {
			if (px < qx) {
				xMin_ = px;
				xMax_ = qx;
			} else {
				xMin_ = qx;
				xMax_ = px;
			}
			yMin_ = py;
			yMax_ = qy;
		} else {
			if (py < qy) {
				xMin_ = py;
				xMax_ = qy;
			} else {
				yMin_ = qy;
				yMax_ = py;
			}
			xMin_ = px;
			xMax_ = qx;
		}
	}

	float a = xMax_ - xMin_;
	float b = yMax_ - yMin_;
	a = a*a;
	b = b*b;
	l_ = a+b;

	dimension = 1;

}

float LineSegment::getXmin() const {
	// IMPLEMENT ME
	return xMin_; // dummy
}

float LineSegment::getXmax() const {
	// IMPLEMENT ME
	return xMax_; // dummy
}

float LineSegment::getYmin() const {
	// IMPLEMENT ME
	return yMin_; // dummy
}

float LineSegment::getYmax() const {
	// IMPLEMENT ME
	return yMax_; // dummy
}

float LineSegment::length() const {
	// IMPLEMENT ME
	return l_; // dummy
}

void LineSegment::translate(float x, float y) {
	xMin_ = xMin_ + x;
	xMax_ = xMax_ + x;
	yMin_ = yMin_ + y;
	yMax_ = yMax_ + y;
}

void LineSegment::rotate() {
	float cx = (xMin_+xMax_)/2;
	float cy = (yMin_+yMax_)/2;
	xMin_ = xMin_ - cx;
	yMin_ = yMin_ - cy;
	xMax_ = xMax_ - cx;
	yMax_ = yMax_ - cy;
	float xTemp = xMin_;
	float yTemp = yMin_;
	xMin_ = xMin_ - yTemp;
	yMin_ = xTemp;
	xTemp = xMax_;
	yTemp = yMax_;
	xMax_ = xMax_ - yTemp;
	yMax_ = xTemp;
	xMin_ = xMin_ + cx;
	yMin_ = yMin_ + cy;
	xMax_ = xMax_ + cx;
	yMax_ = yMax_ + cy;
}

bool LineSegment::contains(const Point& p) const {
//	const float epsilon = 0.001f;
//	float a = (yMax - yMin)/(xMax - xMin);
//	float b = yMin - (a * xMin);
//	if (fabs(p.getY() - (a * p.getX() + b)) < epsilon) {
//		return true;
//	}
	return false;
}

void LineSegment::scale(float f) {
	///////////////////////////////////////////////////////////////
}

int LineSegment::dim() const {
	return dimension;
}

// ============ TwoDShape class ================

//TwoDShape::TwoDShape(){} // REMOVE ME

TwoDShape::TwoDShape(int d) : Shape(d) {
	if (d < 0) {
		throw std::invalid_argument("Shape depth cannot be negative");
	} else {
		depth2 = d;
	}
}

float TwoDShape::area() const {
	// IMPLEMENT ME
	return -999; // dummy
}

void TwoDShape::translate(float x, float y) {
	
}

void TwoDShape::rotate() {
	// IMPLEMENT ME
}

void TwoDShape::scale(float f) {

}

bool TwoDShape::contains(const Point& p) const {
	return false; //dummy
}

int TwoDShape::dim() const {
	return dimension;
}

// ============== Rectangle class ================

Rectangle::Rectangle(const Point& p, const Point& q) : TwoDShape(p.getDepth()) {
	float px = p.getX();
	float py = p.getY();
	float qx = q.getX();
	float qy = q.getY();

	if (p.getDepth() != q.getDepth()) {
		throw std::invalid_argument("Points p and q must have same depth");
	} else if (px == qx && py == qy) {
		throw std::invalid_argument("Both points cannot have the same coordinates");
	} else if (px == qx || py == qy) {
		throw std::invalid_argument("Points cannot be adjacent");
	} else {
		if (px > qx) {
			xMin = qx;
			xMax = px;
		} else {
			xMin = px;
			xMax = qx;
		}
		if (py > qy) {
			yMin = qy;
			yMax = py;
		} else {
			yMin = py;
			yMax = qy;
		}
	}
}

float Rectangle::getXmin() const {
	// IMPLEMENT ME
	return xMin; // dummy
}

float Rectangle::getYmin() const {
	// IMPLEMENT ME
	return yMin; // dummy
}

float Rectangle::getXmax() const {
	// IMPLEMENT ME
	return xMax; // dummy
}

float Rectangle::getYmax() const {
	// IMPLEMENT ME
	return yMax; // dummy
}

void Rectangle::translate(float x, float y){};
void Rectangle::rotate() {};
bool Rectangle::contains(const Point& p) const {
	return false;
};
void Rectangle::scale(float f){};

// ================== Circle class ===================

Circle::Circle(const Point& c, float r) : TwoDShape(c.getDepth()) {
	if (r < 1) {
		throw std::invalid_argument("Radius cannot be 0 or negative");
	} else {
		x_ = c.getX();
		y_ = c.getY();
		rad = r;
	}
}

float Circle::getX() const {
	// IMPLEMENT ME
	return x_; // dummy
}

float Circle::getY() const {
	// IMPLEMENT ME
	return y_; // dummy
}

float Circle::getR() const {
	// IMPLEMENT ME
	return rad; // dummy
}

void Circle::translate(float x, float y){};
void Circle::rotate() {};
bool Circle::contains(const Point& p) const {
	return false;
};
void Circle::scale(float f){};

// ================= Scene class ===================

Scene::Scene() {
	// IMPLEMENT ME
}

void Scene::addObject(std::shared_ptr<Shape> ptr) {
	// IMPLEMENT ME
}

void Scene::setDrawDepth(int depth) {
	// IMPLEMENT ME
}

std::ostream& operator<<(std::ostream& out, const Scene& s) {
	// IMPLEMENT ME
	return out;
}
