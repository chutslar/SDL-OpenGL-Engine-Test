#include <fstream>
#include <stdexcept>
#include "Utility.h"


// Code from: http://insanecoding.blogspot.com/2011/11/how-to-read-in-file-in-c.html
std::string util::get_file_contents(const char *filename)
{
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize((const unsigned int)in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}

	char buf[512];
	snprintf(buf, 512, "File not found: %s", filename);
	throw(std::invalid_argument(buf));
}
