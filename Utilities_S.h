#ifndef UTILITIES_S_H
#define UTILITIES_S_H
#include "Utilities.h"
class Coord
{
public:
	Coord(int rr, int cc, int ss) : m_row(rr), m_col(cc), m_step(ss) {}
	Coord(int rr, int cc) : m_row(rr), m_col(cc) {}
	int r() const { return m_row; }
	int c() const { return m_col; }
	int s() const { return m_step; }
private:
	int m_row;
	int m_col;
	int m_step;
};

class room
{
public:
	room() {
		m_top = randInt(1, 9);
		m_left = randInt(1, 54);
		m_bottom = m_top + randInt(4, 8);
		m_right = m_left + randInt(4, 15);
		if ((m_bottom - m_top) * 2 > m_right - m_left)
		{
			if (randInt(1, 3) != 1)
			{
				m_bottom = m_top + randInt(4, 8);
				m_right = m_left + randInt(8, 15);
			}
		}

	}
	int top() { return m_top; }
	int left() { return m_left; }
	int bottom() { return m_bottom; }
	int right() { return m_right; }
private:
	int m_top;
	int m_left;
	int m_bottom;
	int m_right;
};
#endif // !UTILITIES_S_H
