#pragma once
#include <string>

class Shader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath);

	void useProgram();
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	inline unsigned int getRendererID() const { return m_RendererID; }
	void  deleteProgram();
private:
	void checkCompileErrors(unsigned int shader, std::string type);

private:
	unsigned int m_RendererID;
};