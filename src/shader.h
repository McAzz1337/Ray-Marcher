#include <cstdint>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

class Shader {
	uint32_t handle;
	std::unordered_map<std::string, int> uniforms;

public:
	Shader(const std::string& path);

	int uniformLocation(const char* name);

	void uniform1f(const char* name, float value);
	void uniform2f(const char* name, const glm::vec2& value);
	void uniform1ui(const char* name, uint32_t value);
	void uniformMatrix4(const char* name, const glm::mat4& mat);

	void bind();

private:
	static uint32_t compileShader(const std::string& src, int type);
	static uint32_t linkProgram(uint32_t vertexShader, uint32_t fragmentShader);
};
