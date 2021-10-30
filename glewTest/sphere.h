/******************************************
BRENNEN GREEN
CS535 FALL 2021
SPHERE.H
Implements the SolidSphere class based
off the Shape primitive.
******************************************/
#pragma once
#include "shape.h"
#include "vector3f.h"


class SolidSphere : public Shape {
public:
	SolidSphere(void);
	SolidSphere(GLdouble radius, GLint slices, GLint stacks);
	void render(void);

private:
	int m_num_indices;
	GLdouble m_size;
};

SolidSphere::SolidSphere(void)
{
}

SolidSphere::SolidSphere(GLdouble radius, GLint slices, GLint stacks) {
	float PI = 3.14;
	GLfloat theta = 0;
	GLfloat phi = 0;
	GLfloat d_theta = PI / (float)stacks;
	GLfloat d_phi = 2 * PI / (float)slices;
	m_vertices = new GLfloat[slices * (stacks + 1) * NUM_COORDS];
	m_normals = new GLfloat[slices * (stacks + 1) * NUM_COORDS];
	m_num_indices = slices * stacks * 4;
	m_indices = new GLushort[m_num_indices];
	m_size = radius;

	// Generate the vertices and normals for the sphere.
	int index = 0;
	for (int latitude = 0; latitude <= stacks; latitude++) {
		phi = 0;
		for (int longitude = 0; longitude < slices; longitude++) {
			GLfloat x = sin(theta) * sin(phi);
			GLfloat y = cos(theta);
			GLfloat z = sin(theta) * cos(phi);
			Vector3f n(x, y, z);
			n.normalize();
			m_vertices[index] = x;
			m_normals[index++] = n.x;
			m_vertices[index] = y;
			m_normals[index++] = n.y;
			m_vertices[index] = z;
			m_normals[index++] = n.z;
			phi += d_phi;
		}
		theta += d_theta;
	}

	// Generate the face indices for the sphere.
	index = 0;
	for (int latitude = 0; latitude < stacks; latitude++) {
		for (int longitude = 0; longitude < slices; longitude++) {
			int last_index = slices * latitude + longitude;
			int current_index = slices * (latitude + 1) + longitude;
			m_indices[index++] = last_index;
			m_indices[index++] = current_index;
			m_indices[index++] = current_index + 1;
			m_indices[index++] = last_index + 1;
		}
	}
}

void SolidSphere::render(void) {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	// Set array pointers
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(NUM_COORDS, GL_FLOAT, 0, m_vertices);
	glNormalPointer(GL_FLOAT, 0, m_normals);

	glScalef(m_size, m_size, m_size);
	glDrawElements(GL_QUADS, m_num_indices, GL_UNSIGNED_SHORT, m_indices);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	glPopMatrix();
}