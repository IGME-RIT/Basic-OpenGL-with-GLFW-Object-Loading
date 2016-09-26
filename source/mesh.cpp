/*
Title: DMaterials
File Name: shape.cpp
Copyright ? 2016
Author: David Erbelding
Written under the supervision of David I. Schwartz, Ph.D., and
supported by a professional development seed grant from the B. Thomas
Golisano College of Computing & Information Sciences
(https://www.rit.edu/gccis) at the Rochester Institute of Technology.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "../header/mesh.h"

Mesh::Mesh(std::vector<Vertex3dUVNormal> vertices, std::vector<unsigned int> indices)
{
	m_vertices = vertices;
	m_indices = indices;
	// Create the shape by setting up buffers

	// Set up vertex buffer
	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex3dUVNormal), &m_vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Set up index buffer
	glGenBuffers(1, &m_indexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_indexBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Mesh::Mesh(std::string filePath)
{

    std::ifstream file(filePath);

    // Check if the file exists
    if (!file.good())
    {
        // If we encounter an error, print a message and return false.
        std::cout << "Can't read file: " << filePath << std::endl;
        return;
    }

    // These will contain our vertex data
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;

    // Now we have to process the string . . .
    std::ifstream  fin(filePath);
    std::string    line;

    // Loop over all lines
    while (std::getline(fin, line))
    {
        // vertex positions
        if (strncmp("v ", &line[0], 2) == 0)
        {
            strtok(&line[0], " ");
            float x = std::stof(strtok(0, " "));
            float y = std::stof(strtok(0, " "));
            float z = std::stof(strtok(0, " "));
            vertices.push_back(glm::vec3(x, y, z));
        }
        // texture coordinates
        else if (strncmp("vt", &line[0], 2) == 0)
        {
            strtok(&line[0], " ");
            float u = std::stof(strtok(0, " "));
            float v = std::stof(strtok(0, " "));
            uvs.push_back(glm::vec2(u, v));
        }
        // vertex normals
        else if (strncmp("vn", &line[0], 2) == 0)
        {
            strtok(&line[0], " ");
            float x = std::stof(strtok(0, " "));
            float y = std::stof(strtok(0, " "));
            float z = std::stof(strtok(0, " "));
            normals.push_back(glm::vec3(x, y, z));
        }
        // faces (these should be last in the file, so we can just interpret them immediately)
        else if (strncmp("f", &line[0], 1) == 0)
        {
            // make some faces :P
            std::vector<int> face;

            char* token = strtok(&line[0], " ");
            while ((token = strtok(0, "/")) != 0)
            {
                // split up data (important, obj file indexing starts at 1, so we have to subtract 1 here or bad things happen)
                int i = std::stoi(token) - 1;
                glm::vec3 vp = vertices[i];

                token = strtok(0, "/");
                int j = std::stoi(token) - 1;
                glm::vec2 vt = uvs[j];

                token = strtok(0, " ");
                int k = std::stoi(token) - 1;
                glm::vec3 vn = normals[k];

                bool newVertex = false;

                // does this vertex exist already?
                for (int i = 0; i < m_vertices.size(); i++)
                {
                    Vertex3dUVNormal other = m_vertices[i];
                    // if match found
                    if (vp == other.m_position &&
                        vt == other.m_texCoord &&
                        vn == other.m_normal)
                    {
                        // reuse the index for this face and stop iterating
                        face.push_back(i);
                        newVertex = true;
                        break;
                    }
                }

                // if not a new vertex, create and add it
                if (!newVertex)
                {
                    // the index for this vertex will be at the index at the end of the vector
                    face.push_back(m_vertices.size());
                    m_vertices.push_back(Vertex3dUVNormal(vp, vt, vn));
                }
            }

            // face now has indices

            // add the first 3 indices of the face to our index collection to form a triangle
            for (int i = 0; i < 3; i++)
            {
                m_indices.push_back(face[i]);
            }
            // the face is a quad, add the last 3 vertices in reverse order of appearance
            if (face.size() == 4)
            {
                m_indices.push_back(face[0]);
                m_indices.push_back(face[2]);
                m_indices.push_back(face[3]);
            }


            // we're done here
        }
        // other
        else 
        {
            std::cout << line << std::endl;
        }
    }


    // After all of that nonsense close the file
    fin.close();

    // create buffers for opengl just like normal

	// Set up vertex buffer
    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex3dUVNormal), &m_vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Set up index buffer
    glGenBuffers(1, &m_indexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_indexBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Mesh::~Mesh()
{
	// Clear buffers for the shape object when done using them.
	glDeleteBuffers(1, &m_vertexBuffer);
	glDeleteBuffers(1, &m_indexBuffer);
}



void Mesh::Draw()
{
	// Previously, we multiplied each vertex one by one, but now we just have to send the world matrix to the gpu.


	// Bind the vertex buffer and set the Vertex Attribute.
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3dUVNormal), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3dUVNormal), (void*)sizeof(glm::vec3));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex3dUVNormal), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Bind index buffer to GL_ELEMENT_ARRAY_BUFFER, and enable vertex attribute
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

	// Draw all indices in the index buffer
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, (void*)0);

	// Disable vertex attribute and unbind index buffer.
	glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
