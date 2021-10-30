/******************************************
BRENNEN GREEN
CS535 FALL 2021
CONE.H
Implements the SolidCone class based
off the Shape primitive.
******************************************/
#pragma once
#include "shape.h"
#include "vector3f.h"

class SolidCone : public Shape {
public:
	SolidCone(void);
	SolidCone(GLdouble base, GLdouble height, GLint slices, GLint stacks);

	void render();

private:
	int m_num_indices;
};

SolidCone::SolidCone(void)
{
}

SolidCone::SolidCone(GLdouble base, GLdouble height, GLint slices, GLint stacks) {
	float PI = 3.14159;
	float angle_step = 2.0 * PI / (float)slices;
	int iPivot = 0;
	float slope_step = height / (float)stacks;
	float radius_step = base / (float)stacks;
	int num_vertices = slices * (stacks + 1) * 3 + slices * 3 + 3;
	m_num_indices = slices * stacks * 6 + slices * 3; 
	m_vertices = new GLfloat[num_vertices];
	m_normals = new GLfloat[num_vertices];
	m_indices = new GLushort[m_num_indices];
	float epsilon = 0.2;
	float z = 0;
	int index = 0;

	for (int rows = 0; rows < (stacks + 1); rows++) {
		for (float angle = 0.0; angle < (2.0 * PI - epsilon); angle += angle_step) {
			GLfloat x = base * cos(angle);
			GLfloat y = base * sin(angle);
			Vector3f n(x * height / base, y * height / base, base / height);
			n.normalize();
			m_vertices[index] = x;
			m_normals[index++] = n.x;
			m_vertices[index] = y;
			m_normals[index++] = n.y;
			m_vertices[index] = z;
			m_normals[index++] = n.z;
		}
		z += slope_step;
		base -= radius_step;
	}

	int seg_length = slices * NUM_COORDS;
	for (int i = 0; i < seg_length; i += 3) {
		GLfloat x = m_vertices[i];
		GLfloat y = m_vertices[i + 1];
		GLfloat z = m_vertices[i + 2];
		m_vertices[index] = x;
		m_normals[index++] = 0;
		m_vertices[index] = y;
		m_normals[index++] = 0;
		m_vertices[index] = z;
		m_normals[index++] = -1.0f;
	}

	m_vertices[index] = 0;
	m_normals[index++] = 0;
	m_vertices[index] = 0;
	m_normals[index++] = 0;
	m_vertices[index] = 0;
	m_normals[index++] = -1;

	index = 0;
	for (int rows = 0; rows < stacks; rows++) {
		float this_z = m_vertices[rows * slices * 3 + 2];
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

	int center_index = slices * stacks + slices;
	int base_index = slices * stacks + 1;
	for (int i = 0; i < slices; i++) {
		int bot_left = base_index + i;
		int bot_right = bot_left + 1;
		if (i == slices - 1) {
			bot_right = base_index;
		}
		m_indices[index++] = center_index;
		m_indices[index++] = bot_left;
		m_indices[index++] = bot_right;
	}
}

// Draw the solid cone 
void SolidCone::render() {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, m_vertices);
		glNormalPointer(GL_FLOAT, 0, m_normals);

		glDrawElements(GL_TRIANGLES, m_num_indices, GL_UNSIGNED_SHORT, m_indices);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);

	glPopMatrix();
}