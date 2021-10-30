/******************************************
BRENNEN GREEN
CS535 FALL 2021
SHAPE.H
Implements the Shape primitive class.
******************************************/
#pragma once
#include <freeglut.h>
#include <gl/GL.h>


class Shape
{
public:
	Shape(void);
	~Shape(void);
	virtual void render();

protected:
	GLfloat* m_vertices;
	GLushort* m_indices;
	GLfloat* m_normals;
	static const int NUM_COORDS = 3;
};

Shape::Shape(void) {
}


Shape::~Shape(void) {
	if (m_vertices) {
		delete[] m_vertices;
	}
	if (m_indices) {
		delete[] m_indices;
	}
	if (m_normals) {
		delete[] m_normals;
	}
}

void Shape::render(void) {
}