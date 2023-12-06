#ifndef RESOURCE_BROWSER_H
#define RESOURCE_BROWSER_H

#include <filesystem>
#include <vector>

class TextEdit;

class ResourceBrowser {
public:
	ResourceBrowser();
	void DisplayDirectoryContents(const std::filesystem::path& directoryPath, TextEdit& textEdit);
	void Draw(TextEdit& textEdit);
private:
};

#endif 