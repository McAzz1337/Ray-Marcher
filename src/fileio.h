#pragma once
#include <string>
#include <vector>



	void readFile(const std::string& path, std::string& dst, bool log = false);
	void readFileSplit(const std::string& path, std::vector<std::string>& dst, bool log = false);

	void extractFileName(const std::string& path, std::string& dst, const char del = '/');
	void splitPath(const std::string& path, std::string& pathDst, std::string& fileDst);

	void split(const std::string& src, const char delimeter, std::string* tokens, int size);
	void split(const std::string& src, std::vector<std::string>& dst, const char delimeter);

	uint64_t requestFileSize(const std::string& path);
	void listDirectoryEntries(const std::string& dir, std::vector<std::string>& dst);

	void writeFile(const std::string& path, const std::string& src);


