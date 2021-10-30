/******************************************
BRENNEN GREEN
CS535 FALL 2021
CUB.H
Implements the SolidCube class based
off the Shape primitive.
******************************************/
#pragma once
#include "shape.h"

class SolidCube : public Shape
{
public:
	SolidCube(void);
	SolidCube(GLdouble size);

	virtual void render();

private:
	static const int NUM_VERTICES = 24;
	GLdouble m_size;
};

SolidCube::SolidCube(void) {
}

SolidCube::SolidCube(GLdouble size) {
	GLfloat vertices[] = {
		// Front face
		-0.5f, 0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		// Right face
		0.5f, 0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		// Back face
		0.5f, 0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f,
		// Left face
		-0.5f, 0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		// Top face
		-0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, -0.5f,
		// Bottom face
		-0.5f, -0.5f, 0.5f,
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, 0.5f,
	};

	GLfloat normals[] = {
		// Front
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
		// Right
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		// Back
		0, 0, -1,
		0, 0, -1,
		0, 0, -1,
		0, 0, -1,
		// Left
		-1, 0, 0,
		-1, 0, 0,
		-1, 0, 0,
		-1, 0, 0,
		// Top
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		// Bottom
		0, -1, 0,
		0, -1, 0,
		0, -1, 0,
		0, -1, 0,
	};

	GLushort indices[] = {
		0, 1, 2, 3, // Front
		4, 5, 6, 7, // Right
		8, 9, 10, 11, // Back
		12, 13, 14, 15, // Left
		16, 17, 18, 19, // Top
		20, 21, 22, 23, // Bottom
	};

	// Copy the vertices, normals, and indices to the tables.
	m_vertices = new GLfloat[NUM_VERTICES * NUM_COORDS];
	m_normals = new GLfloat[NUM_VERTICES * NUM_COORDS];
	m_indices = new GLushort[NUM_VERTICES];
	for (int i = 0; i < NUM_VERTICES * NUM_COORDS; i += 3) {
		m_vertices[i] = vertices[i];
		m_vertices[i + 1] = vertices[i + 1];
		m_vertices[i + 2] = vertices[i + 2];
		m_normals[i] = normals[i];
		m_normals[i + 1] = normals[i + 1];
		m_normals[i + 2] = normals[i + 2];
		m_indices[i / 3] = indices[i / 3];
	}

	m_size = size;
}

void SolidCube::render(void) {
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(NUM_COORDS, GL_FLOAT, 0, m_vertices);
	glNormalPointer(GL_FLOAT, 0, m_normals);

	// Scale to the given size, draw and restore previous scaling
	glScalef(m_size, m_size, m_size);
	glDrawElements(GL_QUADS, NUM_VERTICES, GL_UNSIGNED_SHORT, m_indices);

	glPopMatrix();
}



