#ifndef MD5_HPP
#define MD5_HPP

#include <string>

//declared and defined at md5c.c
extern "C" void compute_md5(unsigned char* input_text, unsigned int input_size, unsigned char output_digest[16]);

std::string md5_string(const std::string& text)
{
	unsigned char digest[16]{0};
	compute_md5(reinterpret_cast<unsigned char*>(const_cast<char*>(text.c_str())), text.size(), digest);
    std::string output(32, ' ');
    char* ptr = const_cast<char*>(output.c_str());
	for (size_t i = 0; i < 16; ++i, ptr += 2)
		std::sprintf(ptr, "%02x", digest[i]);
	return output;
}

void md5_digest(const std::string& text, unsigned char digest[16])
{
    compute_md5(reinterpret_cast<unsigned char*>(const_cast<char*>(text.c_str())), text.size(), digest);
}

#endif /* MD5_HPP */