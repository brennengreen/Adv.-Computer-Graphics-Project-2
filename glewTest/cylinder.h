/******************************************
BRENNEN GREEN
CS535 FALL 2021
CYLINDER.H
Implements the SolidCylinder class based
off the Shape primitive.
******************************************/
#pragma once
#include "shape.h"
#include "vector3f.h"

class SolidCylinder : public Shape {
public:
	SolidCylinder();

	SolidCylinder(GLdouble base, GLdouble height, GLint slices, GLint stacks);
	void render(void);

private:
	int m_num_indices;
};

SolidCylinder::SolidCylinder(void) {

}

SolidCylinder::SolidCylinder(GLdouble base, GLdouble height, GLint slices, GLint stacks) {
	float PI = 3.14159;
	float angle_step = 2.0 * PI / (float)slices;
	int iPivot = 0;
	float slope_step = height / (float)stacks;
	m_num_indices = (slices * stacks) * 6 + slices * 6;
	int num_vertices = slices * (stacks + 1) + slices * 2 + 2;
	
	m_vertices = new GLfloat[num_vertices * NUM_COORDS];
	m_normals = new GLfloat[num_vertices * NUM_COORDS];
	m_indices = new GLushort[m_num_indices];
	int index = 0;
	float epsilon = 0.2f;
	GLfloat z = 0;
	for (int rows = 0; rows < stacks + 1; rows++) {
		for (float angle = 0.0; angle < (2.0 * PI - epsilon); angle += angle_step) {
			GLfloat x = cos(angle);
			GLfloat y = sin(angle);
			Vector3f n(x, y, 0);
			n.normalize();
			m_vertices[index] = x;
			m_normals[index++] = n.x;
			m_vertices[index] = y;
			m_normals[index++] = n.y;
			m_vertices[index] = z;
			m_normals[index++] = n.z;
		}
		z += slope_step;
	}

	for (int cap = 0; cap < 2; cap++) {
		int cap_index = 0;
		if (cap == 1) {
			cap_index = slices * stacks * NUM_COORDS;
		}
		for (int i = 0; i < slices * NUM_COORDS; i += 3) {
			GLfloat x = m_vertices[cap_index + i];
			GLfloat y = m_vertices[cap_index + i + 1];
			z = m_vertices[cap_index + i + 2];
			m_vertices[index] = x;
			m_normals[index++] = 0;
			m_vertices[index] = y;
			m_normals[index++] = 0;
			m_vertices[index] = z;
			if (cap == 0) {
				m_normals[index++] = -1.0f;
			}
			else {
				m_normals[index++] = 1.0f;
			}
		}
		m_vertices[index] = 0;
		m_normals[index++] = 0;
		m_vertices[index] = 0;
		m_normals[index++] = 0;
		m_vertices[index] = z;
		if (cap == 0) {
			m_normals[index++] = -1.0f;
		}
		else {
			m_normals[index++] = 1.0f;
		}
	}

	index = 0;
	for (int rows = 0; rows < stacks; rows++) {
		float current_z = m_vertices[rows * slices * 3 + 2];
		for (int i = 0; i < slices; i++) {
			int bot_left = rows * slices + i;
			int top_left = (rows + 1) * slices + i;
			int bot_right = bot_left + 1;
			int top_right = top_left + 1;
			if (i == slices - 1) {
				bot_right = rows * slices;
				top_right = (rows + 1) * slices;
			}
			m_indices[index++] = top_left;
			m_indices[index++] = bot_left;
			m_indices[index++] = bot_right;

			m_indices[index++] = top_left;
			m_indices[index++] = bot_right;
			m_indices[index++] = top_right;
		}
	}

	for (int cap = 0; cap < 2; cap++) {
		int cap_index = slices * (stacks + 1);
		int center_index = slices * (stacks + 2);
		if (cap == 1) {
			cap_index = slices * (stacks + 2) + 1;
			center_index = slices * (stacks + 3) + 1;
		}
		for (int i = 0; i < slices; i++) {
			int bot_left = cap_index + i;
			int bot_right = bot_left + 1;
			if (i == slices - 1) {
				bot_right = cap_index;
			}
			m_indices[index++] = center_index;
			m_indices[index++] = bot_left;
			m_indices[index++] = bot_right;
		}
	}
}

void SolidCylinder::render() {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glVertexPointer(NUM_COORDS, GL_FLOAT, 0, m_vertices);
		glNormalPointer(GL_FLOAT, 0, m_normals);

		glDrawElements(GL_TRIANGLES, m_num_indices, GL_UNSIGNED_SHORT, m_indices);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
	glPopMatrix();

}