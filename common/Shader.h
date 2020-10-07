#ifndef SHADER_H
#define SHADER_H
//#include "stdafx.h"
#include <vector>
#include <GL/glew.h>

class CShader
{
public:
	GLuint getID();
	~CShader(); 
	static CShader* createShaderProgram(const char* pVertex_file_path = nullptr,
										const char* pTesselationControl_file_path = nullptr,
										const char* pTesselationEvaluation_file_path = nullptr,
										const char* pGeometry_file_path = nullptr, 
										const char* pFragment_file_path = nullptr,
										const char* pCompute_file_path = nullptr);
	static std::vector<CShader*> getShaders();

private:
	GLuint ID;
	
	CShader(int id);

};

#endif
