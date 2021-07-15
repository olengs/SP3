#include "MeshBuilder.h"
#include <GL\glew.h>
#include <vector>
#include "Vertex.h"
#include "MyMath.h"
#include "LoadOBJ.h"
#include <iostream>
using namespace std;
/******************************************************************************/
/*!
\brief
Generate the vertices of a reference Axes; Use red for x-axis, green for y-axis, blue for z-axis
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - x-axis should start at -lengthX / 2 and end at lengthX / 2
\param lengthY - y-axis should start at -lengthY / 2 and end at lengthY / 2
\param lengthZ - z-axis should start at -lengthZ / 2 and end at lengthZ / 2

\return Pointer to mesh storing VBO/IBO of reference axes
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	v.pos.Set(-1000, 0, 0);
	v.color.Set(1, 0, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(1000, 0, 0);
	v.color.Set(1, 0, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0, -1000, 0);
	v.color.Set(0, 1, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0, 1000, 0);
	v.color.Set(0, 1, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0, 0, -1000);
	v.color.Set(0, 0, 1);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0, 0, 1000);
	v.color.Set(0, 0, 1);
	vertex_buffer_data.push_back(v);

	std::vector<GLuint> index_buffer_data;
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(5);

	Mesh *mesh = new Mesh(meshName);
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_LINES;

	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a crosshair;
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - x-axis should start at -lengthX / 2 and end at lengthX / 2
\param lengthY - y-axis should start at -lengthY / 2 and end at lengthY / 2
\param lengthZ - z-axis should start at -lengthZ / 2 and end at lengthZ / 2

\return Pointer to mesh storing VBO/IBO of reference axes
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateCrossHair(const std::string &meshName, float colour_r, float colour_g, float colour_b, float length)
{
	// Declare the variable to store a vertex and the buffer for storing vertices
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	// Vertex #1
	v.pos.Set(-length, 0, 0);
	v.color.Set(colour_r, colour_g, colour_b);
	vertex_buffer_data.push_back(v);
	// Vertex #2
	v.pos.Set(length, 0, 0);
	v.color.Set(colour_r, colour_g, colour_b);
	vertex_buffer_data.push_back(v);
	// Vertex #3
	v.pos.Set(0, -length, 0);
	v.color.Set(colour_r, colour_g, colour_b);
	vertex_buffer_data.push_back(v);
	// Vertex #4
	v.pos.Set(0, length, 0);
	v.color.Set(colour_r, colour_g, colour_b);
	vertex_buffer_data.push_back(v);

	std::vector<GLuint> index_buffer_data;
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(3);

	Mesh *mesh = new Mesh(meshName);
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_LINES;

	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a quad; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of quad
\param lengthY - height of quad

\return Pointer to mesh storing VBO/IBO of quad
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateQuad(const std::string &meshName, Color color, float length)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	
	v.pos.Set(-0.5f * length,-0.5f * length,0);
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(0, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length,-0.5f * length,0);
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(1.0f, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length, 0.5f * length,0);
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(1.0f, 1.0f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * length, 0.5f * length,0);
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(0, 1.0f);
	vertex_buffer_data.push_back(v);
	
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(0);
	
	Mesh *mesh = new Mesh(meshName);
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;
}

Mesh* MeshBuilder::GenerateSlicedQuad(const std::string& meshName, Color color, float length, unsigned numRow, unsigned numCol)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float width = length / numCol;
	float height = length / numRow;
	int offset = 0;
	for (unsigned i = 0; i < numRow; ++i)
	{
		for (unsigned k = 0; k < numCol; ++k)
		{
			float x_axis = k * width;
			float y_axis = i * height;
			v.pos.Set(x_axis, y_axis, 0);
			v.texCoord.Set(x_axis,y_axis);
			vertex_buffer_data.push_back(v);

			v.pos.Set(x_axis + width, y_axis, 0);
			v.texCoord.Set(x_axis + width, y_axis);
			vertex_buffer_data.push_back(v);

			v.pos.Set(x_axis + width, y_axis + height, 0);
			v.texCoord.Set(x_axis + width, y_axis + height);
			vertex_buffer_data.push_back(v);

			v.pos.Set(x_axis, y_axis + height, 0);
			v.texCoord.Set(x_axis, y_axis + height);
			vertex_buffer_data.push_back(v);

			index_buffer_data.push_back(offset + 0);
			index_buffer_data.push_back(offset + 1);
			index_buffer_data.push_back(offset + 2);
			index_buffer_data.push_back(offset + 0);
			index_buffer_data.push_back(offset + 2);
			index_buffer_data.push_back(offset + 3);
			offset += 4;
		}
	}

	Mesh* mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;
}

SpriteAnimation* MeshBuilder::GenerateSpriteAnimation(const std::string& meshName, unsigned numRow, unsigned numCol, float length)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float width = 1.f / numCol;
	float height = 1.f / numRow;
	int offset = 0;
	for (unsigned i = 0; i < numRow; ++i)
	{
		for (unsigned j = 0; j < numCol; ++j)
		{
			float u1 = j * width;
			float v1 = 1.f - height - i * height;
			v.pos.Set(-0.5f, -0.5f, 0);
			v.texCoord.Set(u1, v1);
			vertex_buffer_data.push_back(v);

			v.pos.Set(0.5f, -0.5f, 0);
			v.texCoord.Set(u1 + width, v1);
			vertex_buffer_data.push_back(v);

			v.pos.Set(0.5f, 0.5f, 0);
			v.texCoord.Set(u1 + width, v1 + height);
			vertex_buffer_data.push_back(v);

			v.pos.Set(-0.5f, 0.5f, 0);
			v.texCoord.Set(u1, v1 + height);
			vertex_buffer_data.push_back(v);

			index_buffer_data.push_back(offset + 0);
			index_buffer_data.push_back(offset + 1);
			index_buffer_data.push_back(offset + 2);
			index_buffer_data.push_back(offset + 0);
			index_buffer_data.push_back(offset + 2);
			index_buffer_data.push_back(offset + 3);
			offset += 4;
		}
	}


	SpriteAnimation* mesh = new SpriteAnimation(meshName,numRow,numCol);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;
}

Mesh* MeshBuilder::GenerateCircle(const std::string& meshName, Color color, unsigned numSlice, float radius)
{
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	Vertex v;

	float degreePerSlice = 360.f / numSlice;
	//float radianPerSlice = Math::DegreeToRadian(360.f) / numSlice;
	for (unsigned slice = 0; slice < numSlice + 1; ++slice)
	{
		float theta = slice * degreePerSlice;
		v.color = color;
		v.normal.Set(0, 0, 1);
		v.pos.Set(radius * cos(Math::DegreeToRadian(theta)), radius * sin(Math::DegreeToRadian(theta)), 0);
		v.texCoord.Set(0.5 * radius * cos(Math::DegreeToRadian(theta)) + 0.5f, 0.5 * radius * sin(Math::DegreeToRadian(theta)) + 0.5f);
		vertex_buffer_data.push_back(v);
		index_buffer_data.push_back(slice * 2);

		v.pos.Set(0, 0, 0);
		v.texCoord.Set(0.5f, 0.5f);
		vertex_buffer_data.push_back(v);
		index_buffer_data.push_back(slice * 2 + 1);

	}
	Mesh* mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	mesh->indexSize = index_buffer_data.size();

	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a cube; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of cube
\param lengthY - height of cube
\param lengthZ - depth of cube

\return Pointer to mesh storing VBO/IBO of cube
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateCube(const std::string &meshName, Color color, float length)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;

	v.pos.Set(-0.5f * length,-0.5f * length,-0.5f * length);
	v.color = color;
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length,-0.5f * length,-0.5f * length);
	v.color = color;
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length, 0.5f * length,-0.5f * length);
	v.color = color;
	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * length, 0.5f * length,-0.5f * length);
	v.color = color;
	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * length,-0.5f * length, 0.5f * length);
	v.color = color;
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length,-0.5f * length, 0.5f * length);
	v.color = color;
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length, 0.5f * length, 0.5f * length);
	v.color = color;
	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * length, 0.5f * length, 0.5f * length);
	v.color = color;
	vertex_buffer_data.push_back(v);
	
	std::vector<GLuint> index_buffer_data;
	index_buffer_data.push_back(7);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(7);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(7);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(7);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(7);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(5);
	
	Mesh *mesh = new Mesh(meshName);
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = 36;
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;
}

Mesh* MeshBuilder::GenerateRing(const std::string &meshName, Color color, unsigned numSlice, float outerR, float innerR)
{
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	Vertex v;

	float degreePerSlice = 360.f / numSlice;
	//float radianPerSlice = Math::DegreeToRadian(360.f) / numSlice;
	for(unsigned slice = 0; slice < numSlice + 1; ++slice)
	{
		float theta = slice * degreePerSlice;
		v.color = color;
		v.normal.Set(0, 1, 0);
		v.pos.Set(outerR * cos(Math::DegreeToRadian(theta)), 0, outerR * sin(Math::DegreeToRadian(theta)));
		vertex_buffer_data.push_back(v);
		
		v.color = color;
		v.normal.Set(0, 1, 0);
		v.pos.Set(innerR * cos(Math::DegreeToRadian(theta)), 0, innerR * sin(Math::DegreeToRadian(theta)));
		vertex_buffer_data.push_back(v);
	}
	for(unsigned slice = 0; slice < numSlice + 1; ++slice)
	{
		index_buffer_data.push_back(2 * slice + 0);
		index_buffer_data.push_back(2 * slice + 1);
		//index_buffer_data.push_back(2 * slice + 3);
		//index_buffer_data.push_back(2 * slice + 4);
		//index_buffer_data.push_back(2 * slice + 3);
		//index_buffer_data.push_back(2 * slice + 2);
	}

	Mesh *mesh = new Mesh(meshName);
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	mesh->indexSize = index_buffer_data.size();

	return mesh;
}

float sphereX(float phi, float theta)
{
	return cos(Math::DegreeToRadian(phi)) * cos(Math::DegreeToRadian(theta));
}
float sphereY(float phi, float theta)
{
	return sin(Math::DegreeToRadian(phi));
}
float sphereZ(float phi, float theta)
{
	return cos(Math::DegreeToRadian(phi)) * sin(Math::DegreeToRadian(theta));
}

Mesh* MeshBuilder::GenerateSphere(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float radius)
{
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	
	float degreePerStack = 180.f / numStack;
	float degreePerSlice = 360.f / numSlice;
	//float radianPerSlice = Math::DegreeToRadian(360.f) / numSlice;

	for(unsigned stack = 0; stack < numStack + 1; ++stack) //stack //replace with 180 for sphere
	{
		float phi = -90.f + stack * degreePerStack;
		for(unsigned slice = 0; slice < numSlice + 1; ++slice) //slice
		{
			float theta = slice * degreePerSlice;
			Vertex v;
			v.pos.Set(radius * sphereX(phi, theta), radius * sphereY(phi, theta), radius * sphereZ(phi, theta));
			v.color = color;
			v.normal.Set(sphereX(phi, theta), sphereY(phi, theta), sphereZ(phi, theta));
			vertex_buffer_data.push_back(v);
		}
	}
	for(unsigned stack = 0; stack < numStack; ++stack)
	{
		for(unsigned slice = 0; slice < numSlice + 1; ++slice)
		{
			index_buffer_data.push_back((numSlice + 1) * stack + slice + 0);
			index_buffer_data.push_back((numSlice + 1) * (stack + 1) + slice + 0);
			//index_buffer_data.push_back((numSlice + 1) * stack + slice + 1);
			//index_buffer_data.push_back((numSlice + 1) * (stack + 1) + slice + 1);
			//index_buffer_data.push_back((numSlice + 1) * stack + slice + 1);
			//index_buffer_data.push_back((numSlice + 1) * (stack + 1) + slice + 0);
		}
	}

	Mesh *mesh = new Mesh(meshName);

	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();

	return mesh;
}

Mesh* MeshBuilder::GenerateCone(const std::string &meshName, Color color, unsigned numSlice, float radius, float height)
{
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	
	Vertex v;
	float degreePerSlice = 360.f / numSlice;

	for(unsigned slice = 0; slice < numSlice + 1; ++slice) //slice
	{
		float theta = slice * degreePerSlice;
		Vector3 normal(height * cos(Math::DegreeToRadian(theta)), radius, height * sin(Math::DegreeToRadian(theta)));
		normal.Normalize();

		v.pos.Set(radius * cos(Math::DegreeToRadian(theta)), 0, radius * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		v.normal = normal;
		vertex_buffer_data.push_back(v);
		
		v.pos.Set(0, height, 0);
		v.color = color;
		v.normal = normal;
		vertex_buffer_data.push_back(v);
	}
	for(unsigned slice = 0; slice < numSlice + 1; ++slice)
	{
		index_buffer_data.push_back(slice * 2 + 0);
		index_buffer_data.push_back(slice * 2 + 1);
	}

	Mesh *mesh = new Mesh(meshName);

	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();

	return mesh;
}

Mesh* MeshBuilder::GenerateOBJ(const std::string &meshName, const std::string &file_path)
{
	std::vector<Position> vertices;
	std::vector<TexCoord> uvs;
	std::vector<Vector3> normals;
	bool success = LoadOBJ(file_path.c_str(), vertices, uvs, normals);
	if(!success)
		return NULL;

	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	
	IndexVBO(vertices, uvs, normals, index_buffer_data, vertex_buffer_data);

	Mesh *mesh = new Mesh(meshName);
	
	mesh->mode = Mesh::DRAW_TRIANGLES;
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();

	return mesh;
}

Mesh* MeshBuilder::GenerateText(const std::string &meshName, unsigned numRow, unsigned numCol)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	
	float width = 1.f / numCol;
	float height = 1.f / numRow;
	int offset = 0;
	for(unsigned i = 0; i < numRow; ++i)
	{
		for(unsigned j = 0; j < numCol; ++j)
		{
			float u1 = j * width;
			float v1 = 1.f - height - i * height;
			v.pos.Set(-0.5f, -0.5f, 0);
			v.texCoord.Set(u1, v1);
			vertex_buffer_data.push_back(v);

			v.pos.Set(0.5f, -0.5f, 0);
			v.texCoord.Set(u1 + width, v1);
			vertex_buffer_data.push_back(v);

			v.pos.Set(0.5f, 0.5f, 0);
			v.texCoord.Set(u1 + width, v1 + height);
			vertex_buffer_data.push_back(v);

			v.pos.Set(-0.5f, 0.5f, 0);
			v.texCoord.Set(u1, v1 + height);
			vertex_buffer_data.push_back(v);
	
			index_buffer_data.push_back(offset + 0);
			index_buffer_data.push_back(offset + 1);
			index_buffer_data.push_back(offset + 2);
			index_buffer_data.push_back(offset + 0);
			index_buffer_data.push_back(offset + 2);
			index_buffer_data.push_back(offset + 3);
			offset += 4;
		}
	}
	
	Mesh *mesh = new Mesh(meshName);
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;
}

Mesh* MeshBuilder::GenerateSkyPlane(const std::string& meshName, Color color, int slices, float PlanetRadius, float AtmosphereRadius, float hTile, float vTile)
{
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	slices = Math::Clamp(slices, 1, 256);
	// Calculate some values we will need
	float planeSize = 2.0f *
		(float)sqrtf((AtmosphereRadius * AtmosphereRadius)
			+ (PlanetRadius * PlanetRadius));
	float delta = planeSize / (float)slices;
	float texDelta = 2.0f / (float)slices;
	for (int z = 0; z <= slices; ++z)
	{
		for (int x = 0; x <= slices; ++x)
		{
			float xDist = (-0.5f * planeSize) + ((float)x * delta);
			float zDist = (-0.5f * planeSize) + ((float)z * delta);
			float xHeight = (xDist * xDist) / AtmosphereRadius;
			float zHeight = (zDist * zDist) / AtmosphereRadius;
			float height = xHeight + zHeight;
			Vertex tv; // temporary vertex 
			tv.pos.x = xDist;
			tv.pos.y = 0.0f - height;
			tv.pos.z = zDist;
			// Calculate the texture coordinates
			tv.texCoord.u = hTile * ((float)x * texDelta * 0.5f);
			tv.texCoord.v = vTile * (1.0f - (float)z * texDelta * 0.5f);
			tv.color = color;
			vertex_buffer_data.push_back(tv);
		}
	}
	// Calculate the indices
	int index = 0; // Indices
	for (int i = 0; i < slices; ++i)
	{
		for (int j = 0; j < slices; ++j)
		{
			int startvert = (i * (slices + 1) + j);
			index_buffer_data.push_back(startvert); // tri 1
			index_buffer_data.push_back(startvert + 1);
			index_buffer_data.push_back(startvert + slices + 1);
			index_buffer_data.push_back(startvert + 1); // tri 2
			index_buffer_data.push_back(startvert + slices + 2);
			index_buffer_data.push_back(startvert + slices + 1);
		}
	}
	// Calculate the indices
	Mesh* mesh = new Mesh(meshName);
	mesh->mode = Mesh::DRAW_TRIANGLES;
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() *
		sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size()
		* sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->indexSize = index_buffer_data.size();
	return mesh;
}

Mesh* MeshBuilder::GenerateTerrain(const std::string& meshName, const std::string& filename, std::vector<unsigned char>& heightMap)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// To be completed
	// Set a scale factor to size your terrain
	// Load/ read information from the heightmap
	// Calculate the terrainSize
	// Based on heightmap, calculate a scaledHeight
	// Calculate x, y and z of each vertices
	// Set color or normal if necessary
	// Calculate the vertex coordinates
	// Assign to vertex buffer
	// Calculate the indices
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	const float SCALE_FACTOR = 256.0f; //Scale factor == Terrain Height map size
	//Scale Factor is being used for normalization of the mesh.. normalize value

	if (!LoadHeightMap(filename.c_str(), heightMap))
		return NULL;

	unsigned terrainSize = (unsigned)sqrt((double)heightMap.size());

	for (unsigned z = 0; z < terrainSize; ++z)
	{
		for (unsigned x = 0; x < terrainSize; ++x)
		{
			float scaledHeight = (float)heightMap[z * terrainSize + x] / SCALE_FACTOR;
			v.normal.Set(0.f, 1.f, 0.f);
			//Trying to render 2 vertices of a strip at one time.
			//Divide by scale factor.

			//normalize will usually be used for height

			//Vertex position x, y, z
			v.pos.Set(static_cast<float>(x) / terrainSize - 0.5f, scaledHeight, static_cast<float>(z) / terrainSize - 0.5f);

			//Color
			v.color.Set(scaledHeight, scaledHeight, scaledHeight); //For Rendering height map without texture

			//Tex
			v.texCoord.Set((float)x / terrainSize * 8, 1.f - (float)z / terrainSize * 8);
			//v.texCoord.Set((float)x / terrainSize * hTile, 1.f - (float)z / terrainSize * vTile);

			vertex_buffer_data.push_back(v);
		}
	}

	//Indices
	for (unsigned z = 0; z < terrainSize - 1; ++z)
	{
		for (unsigned x = 0; x < terrainSize - 1; ++x)
		{
			index_buffer_data.push_back(terrainSize * z + x + 0); //Tri 1
			index_buffer_data.push_back(terrainSize * (z + 1) + x + 0);
			index_buffer_data.push_back(terrainSize * z + x + 1);

			index_buffer_data.push_back(terrainSize * (z + 1) + (x + 1)); //Tri 2
			index_buffer_data.push_back(terrainSize * z + x + 1);
			index_buffer_data.push_back(terrainSize * (z + 1) + x + 0);
		}
	}

	//Calculate Indices
	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;
	return mesh;
}

Mesh* MeshBuilder::GenerateTerrain(const std::string& meshName, const std::string& filename, std::vector<unsigned char>& heightMap, float hTile, float vTile)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	const float SCALE_FACTOR = 256.0f; //Scale factor == terrain height map size

	if (!LoadHeightMap(filename.c_str(), heightMap)) {
		return NULL;
	}
	unsigned terrainSize = (unsigned)sqrt((double)heightMap.size());
	for (unsigned z = 0; z < terrainSize; ++z) {
		for (unsigned x = 0; x < terrainSize; ++x) {
			float scaledHeight = (float)heightMap[z * terrainSize + x] / SCALE_FACTOR;
			//trying to render 2 vertices in a strip
			//Divide by Scale factor
			//normalize will be normally used for height
			//vertex
			v.pos.Set(static_cast<float>(x) / terrainSize - 0.5f, scaledHeight, static_cast<float>(z) / terrainSize - 0.5f);
			//color without tex
			v.color.Set(scaledHeight, scaledHeight, scaledHeight);
			//rendering without texture

			//normal
			//v.normal.Set()

			//tex coord 
			//v.texCoord.Set((float)x * 8, 1.f - (float)z / terrainSize * 8);
			v.texCoord.Set((float)x / terrainSize * hTile, 1.f - (float)z / terrainSize * vTile);
			vertex_buffer_data.emplace_back(v);
		}
	}
	//indices
	for (unsigned z = 0; z < terrainSize - 1; ++z) {
		// 255
		for (unsigned x = 0; x < terrainSize - 1; ++x) {
			index_buffer_data.emplace_back(terrainSize * z + x + 0);
			index_buffer_data.emplace_back(terrainSize * (z + 1) + x + 0);//tri 1
			index_buffer_data.emplace_back(terrainSize * z + x + 1);

			index_buffer_data.emplace_back(terrainSize * (z + 1) + x + 1);
			index_buffer_data.emplace_back(terrainSize * z + x + 1); //tri 2
			index_buffer_data.emplace_back(terrainSize * (z + 1) + x + 0);
		}
	}
	// Calculate the indices
	Mesh* mesh = new Mesh(meshName);
	mesh->mode = Mesh::DRAW_TRIANGLES;
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() *
		sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size()
		* sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->indexSize = index_buffer_data.size();
	return mesh;
}

Mesh* MeshBuilder::GenerateTerrain(const std::string& meshName, const std::string& filename, std::vector<unsigned char>& heightMap, TRS terrain)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// To be completed
	// Set a scale factor to size your terrain
	// Load/ read information from the heightmap
	// Calculate the terrainSize
	// Based on heightmap, calculate a scaledHeight
	// Calculate x, y and z of each vertices
	// Set color or normal if necessary
	// Calculate the vertex coordinates
	// Assign to vertex buffer
	// Calculate the indices
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	const float SCALE_FACTOR = 256.0f; //Scale factor == Terrain Height map size
	//Scale Factor is being used for normalization of the mesh.. normalize value

	if (!LoadHeightMap(filename.c_str(), heightMap))
		return NULL;

	unsigned terrainSize = (unsigned)sqrt((double)heightMap.size());

	for (unsigned z = 0; z < terrainSize; ++z)
	{
		for (unsigned x = 0; x < terrainSize; ++x)
		{
			float scaledHeight = (float)heightMap[z * terrainSize + x] / SCALE_FACTOR;
			//v.normal.Set(0.f, 1.f, 0.f);
			//Trying to render 2 vertices of a strip at one time.
			//Divide by scale factor.

			//normalize will usually be used for height

			//Vertex position x, y, z
			v.pos.Set(static_cast<float>(x) / terrainSize - 0.5f, scaledHeight, static_cast<float>(z) / terrainSize - 0.5f);
			v.normal.Set(0, 1, 0);
			//Color
			v.color.Set(scaledHeight, scaledHeight, scaledHeight); //For Rendering height map without texture

			//Tex
			v.texCoord.Set((float)x / terrainSize * 8, 1.f - (float)z / terrainSize * 8);
			//v.texCoord.Set((float)x / terrainSize * hTile, 1.f - (float)z / terrainSize * vTile);

			vertex_buffer_data.push_back(v);
		}
	}

	//Indices
	for (unsigned z = 0; z < terrainSize - 1; ++z)
	{
		for (unsigned x = 0; x < terrainSize - 1; ++x)
		{
			index_buffer_data.push_back(terrainSize * z + x + 0); //Tri 1
			index_buffer_data.push_back(terrainSize * (z + 1) + x + 0);
			index_buffer_data.push_back(terrainSize * z + x + 1);

			index_buffer_data.push_back(terrainSize * (z + 1) + (x + 1)); //Tri 2
			index_buffer_data.push_back(terrainSize * z + x + 1);
			index_buffer_data.push_back(terrainSize * (z + 1) + x + 0);
		}
	}
	for (unsigned z = 0; z < terrainSize; ++z) {
		for (unsigned x = 0; x < terrainSize; ++x) {
			Vector3 pos = Vector3(vertex_buffer_data.at(z * terrainSize + x).pos.x, vertex_buffer_data.at(z * terrainSize + x).pos.y, vertex_buffer_data.at(z * terrainSize + x).pos.z) * terrain.Scale;
			if (z != 0 && x != 0 && z != terrainSize - 1 && x != terrainSize - 1) {
				Vector3 left = Vector3(vertex_buffer_data.at(z * terrainSize + x - 1).pos.x, vertex_buffer_data.at(z * terrainSize + x - 1).pos.y, vertex_buffer_data.at(z * terrainSize + x - 1).pos.z) * terrain.Scale;
				Vector3 right = Vector3(vertex_buffer_data.at(z * terrainSize + x + 1).pos.x, vertex_buffer_data.at(z * terrainSize + x + 1).pos.y, vertex_buffer_data.at(z * terrainSize + x + 1).pos.z) * terrain.Scale;
				Vector3 up = Vector3(vertex_buffer_data.at((z + 1) * terrainSize + x).pos.x, vertex_buffer_data.at((z + 1) * terrainSize + x).pos.y, vertex_buffer_data.at((z + 1) * terrainSize + x).pos.z) * terrain.Scale;
				Vector3 down = Vector3(vertex_buffer_data.at((z - 1) * terrainSize + x).pos.x, vertex_buffer_data.at((z - 1) * terrainSize + x).pos.y, vertex_buffer_data.at((z - 1) * terrainSize + x).pos.z) * terrain.Scale;
				Vector3 average = (right-pos).Cross(up-pos) + (down-pos).Cross(right-pos) + (left-pos).Cross(down-pos) + (up-pos).Cross(left-pos);
				average *= 0.25;
				average.Normalize();
				vertex_buffer_data.at(z * terrainSize + x).normal.Set(average.x,average.y,average.z);
			}
			else if (x == 0 && z == 0) {
				Vector3 right = Vector3(vertex_buffer_data.at(z * terrainSize + x + 1).pos.x, vertex_buffer_data.at(z * terrainSize + x + 1).pos.y, vertex_buffer_data.at(z * terrainSize + x + 1).pos.z) * terrain.Scale;
				Vector3 up = Vector3(vertex_buffer_data.at((z + 1) * terrainSize + x).pos.x, vertex_buffer_data.at((z + 1) * terrainSize + x).pos.y, vertex_buffer_data.at((z + 1) * terrainSize + x).pos.z) * terrain.Scale;
				Vector3 average = (right - pos).Cross(up - pos);
				average.Normalize();
				vertex_buffer_data.at(z * terrainSize + x).normal.Set(average.x, average.y, average.z);
			}
			else if (x == 0 && z == terrainSize - 1) {
				Vector3 right = Vector3(vertex_buffer_data.at(z * terrainSize + x + 1).pos.x, vertex_buffer_data.at(z * terrainSize + x + 1).pos.y, vertex_buffer_data.at(z * terrainSize + x + 1).pos.z) * terrain.Scale;
				Vector3 down = Vector3(vertex_buffer_data.at((z - 1) * terrainSize + x).pos.x, vertex_buffer_data.at((z - 1) * terrainSize + x).pos.y, vertex_buffer_data.at((z - 1) * terrainSize + x).pos.z) * terrain.Scale;
				Vector3 average = (down - pos).Cross(right - pos);
				average.Normalize();
				vertex_buffer_data.at(z * terrainSize + x).normal.Set(average.x, average.y, average.z);
			}
			else if (x == terrainSize - 1&& z == 0) {
				Vector3 up = Vector3(vertex_buffer_data.at((z + 1) * terrainSize + x).pos.x, vertex_buffer_data.at((z + 1) * terrainSize + x).pos.y, vertex_buffer_data.at((z + 1) * terrainSize + x).pos.z) * terrain.Scale;
				Vector3 left = Vector3(vertex_buffer_data.at(z * terrainSize + x - 1).pos.x, vertex_buffer_data.at(z * terrainSize + x - 1).pos.y, vertex_buffer_data.at(z * terrainSize + x - 1).pos.z) * terrain.Scale;
				Vector3 average = (up - pos).Cross(left - pos);
				average.Normalize();
				vertex_buffer_data.at(z * terrainSize + x).normal.Set(average.x, average.y, average.z);
			}
			else if (x == terrainSize - 1 && z == terrainSize - 1) {
				Vector3 down = Vector3(vertex_buffer_data.at((z - 1) * terrainSize + x).pos.x, vertex_buffer_data.at((z - 1) * terrainSize + x).pos.y, vertex_buffer_data.at((z - 1) * terrainSize + x).pos.z) * terrain.Scale;
				Vector3 left = Vector3(vertex_buffer_data.at(z * terrainSize + x - 1).pos.x, vertex_buffer_data.at(z * terrainSize + x - 1).pos.y, vertex_buffer_data.at(z * terrainSize + x - 1).pos.z) * terrain.Scale;
				Vector3 average = (left - pos).Cross(down - pos);
				average.Normalize();
				vertex_buffer_data.at(z * terrainSize + x).normal.Set(average.x, average.y, average.z);
			}
			else if (x == 0) {
				Vector3 right = Vector3(vertex_buffer_data.at(z * terrainSize + x + 1).pos.x, vertex_buffer_data.at(z * terrainSize + x + 1).pos.y, vertex_buffer_data.at(z * terrainSize + x + 1).pos.z) * terrain.Scale;
				Vector3 up = Vector3(vertex_buffer_data.at((z + 1) * terrainSize + x).pos.x, vertex_buffer_data.at((z + 1) * terrainSize + x).pos.y, vertex_buffer_data.at((z + 1) * terrainSize + x).pos.z) * terrain.Scale;
				Vector3 down = Vector3(vertex_buffer_data.at((z - 1) * terrainSize + x).pos.x, vertex_buffer_data.at((z - 1) * terrainSize + x).pos.y, vertex_buffer_data.at((z - 1) * terrainSize + x).pos.z) * terrain.Scale;
				Vector3 average = (right - pos).Cross(up - pos) + (down - pos).Cross(right - pos);
				average *= 0.5;
				average.Normalize();
				vertex_buffer_data.at(z * terrainSize + x).normal.Set(average.x, average.y, average.z);
			}
			else if (x == terrainSize - 1) {
				Vector3 up = Vector3(vertex_buffer_data.at((z + 1) * terrainSize + x).pos.x, vertex_buffer_data.at((z + 1) * terrainSize + x).pos.y, vertex_buffer_data.at((z + 1) * terrainSize + x).pos.z) * terrain.Scale;
				Vector3 down = Vector3(vertex_buffer_data.at((z - 1) * terrainSize + x).pos.x, vertex_buffer_data.at((z - 1) * terrainSize + x).pos.y, vertex_buffer_data.at((z - 1) * terrainSize + x).pos.z) * terrain.Scale;
				Vector3 left = Vector3(vertex_buffer_data.at(z * terrainSize + x - 1).pos.x, vertex_buffer_data.at(z * terrainSize + x - 1).pos.y, vertex_buffer_data.at(z * terrainSize + x - 1).pos.z) * terrain.Scale;
				Vector3 average = (left - pos).Cross(down - pos) + (up - pos).Cross(left - pos);
				average *= 0.5;
				average.Normalize();
				vertex_buffer_data.at(z * terrainSize + x).normal.Set(average.x, average.y, average.z);
			}
			else if (z == 0) {
				Vector3 left = Vector3(vertex_buffer_data.at(z * terrainSize + x - 1).pos.x, vertex_buffer_data.at(z * terrainSize + x - 1).pos.y, vertex_buffer_data.at(z * terrainSize + x - 1).pos.z) * terrain.Scale;
				Vector3 right = Vector3(vertex_buffer_data.at(z * terrainSize + x + 1).pos.x, vertex_buffer_data.at(z * terrainSize + x + 1).pos.y, vertex_buffer_data.at(z * terrainSize + x + 1).pos.z) * terrain.Scale;
				Vector3 up = Vector3(vertex_buffer_data.at((z + 1) * terrainSize + x).pos.x, vertex_buffer_data.at((z + 1) * terrainSize + x).pos.y, vertex_buffer_data.at((z + 1) * terrainSize + x).pos.z) * terrain.Scale;
				Vector3 average = (right - pos).Cross(up - pos) + (up - pos).Cross(left - pos);
				average *= 0.5;
				average.Normalize();
				vertex_buffer_data.at(z * terrainSize + x).normal.Set(average.x, average.y, average.z);

			}
			else {
				Vector3 left = Vector3(vertex_buffer_data.at(z * terrainSize + x - 1).pos.x, vertex_buffer_data.at(z * terrainSize + x - 1).pos.y, vertex_buffer_data.at(z * terrainSize + x - 1).pos.z) * terrain.Scale;
				Vector3 right = Vector3(vertex_buffer_data.at(z * terrainSize + x + 1).pos.x, vertex_buffer_data.at(z * terrainSize + x + 1).pos.y, vertex_buffer_data.at(z * terrainSize + x + 1).pos.z) * terrain.Scale;
				Vector3 down = Vector3(vertex_buffer_data.at((z - 1) * terrainSize + x).pos.x, vertex_buffer_data.at((z - 1) * terrainSize + x).pos.y, vertex_buffer_data.at((z - 1) * terrainSize + x).pos.z) * terrain.Scale;
				Vector3 average = (down - pos).Cross(right - pos) + (left - pos).Cross(down - pos);
				average *= 0.5;
				average.Normalize();
				vertex_buffer_data.at(z * terrainSize + x).normal.Set(average.x, average.y, average.z);
			}
		}
	}


	// Calculate the indices
	Mesh* mesh = new Mesh(meshName);
	mesh->mode = Mesh::DRAW_TRIANGLES;
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() *
		sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size()
		* sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->indexSize = index_buffer_data.size();
	return mesh;
}

Mesh* MeshBuilder::GenerateSkyDome(const std::string& meshName, Color color, unsigned numStack, unsigned numSlice, float radius)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	float degreePerStack = 90.f / numStack;
	float degreePerSlice = 360.f / numSlice;
	float uspacing = 1.0f / (numStack + 1);
	float vspacing = 1.0f / numSlice;
	float prev = 0;
	float texDelta = 2.0f / numSlice;
	for (unsigned stack = 0; stack < numStack + 1; ++stack) {
		float phi = 90 + stack * degreePerStack;
		for (unsigned slice = 0; slice < numSlice + 1 ; ++slice) {
			float theta = slice * degreePerSlice;
			v.color = color;
			v.pos.Set(radius * sphereX(phi, theta), radius * sphereY(phi, theta), radius * sphereZ(phi, theta));
			v.normal = (Vector3(0,0,0) - Vector3(v.pos.x, v.pos.y, v.pos.z)).Normalized();
			v.texCoord.u = (slice + 1) * vspacing;
			v.texCoord.v = (1.f - (stack + 1) * uspacing);
			if (v.texCoord.u - prev > 0.9f) {
				v.texCoord.u += 1.f;
			}
			prev = v.texCoord.u;
			//v.texCoord.u = ((float)stack * texDelta * 0.5f);
			//v.texCoord.v = (1.0f - (float)slice * texDelta * 0.5f);
			//std::cout << v.texCoord.u << "," << v.texCoord.v << " ";
			vertex_buffer_data.push_back(v);
		}
	}
	for (unsigned stack = 0; stack < numStack; ++stack) {
		for (unsigned slice = 0; slice < numSlice + 1; ++slice) {
			index_buffer_data.push_back(stack * (numSlice + 1) + slice);
			index_buffer_data.push_back((stack + 1) * (numSlice + 1) + slice);
		}
	}
	Mesh* mesh = new Mesh(meshName);
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	return mesh;
}