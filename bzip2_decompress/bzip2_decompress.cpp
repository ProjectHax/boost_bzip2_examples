#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/iostreams/filter/bzip2.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>

bool bzip2_decompress(boost::filesystem::path in_path, boost::filesystem::path out_path, bool slow = false)
{
	try
	{
		boost::filesystem::ifstream ifs(in_path, std::ios::in | std::ios::binary);
		std::string temp((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
		ifs.close();

		boost::filesystem::fstream fs(out_path, std::ios::out | std::ios::binary);

		boost::iostreams::filtering_streambuf<boost::iostreams::output> out;
		out.push(boost::iostreams::bzip2_decompressor(slow));
		out.push(fs);

		boost::iostreams::copy(boost::iostreams::basic_array_source<char>(&temp[0], temp.size()), out);
	}
	catch(std::exception & e)
	{
		std::cout << e.what() << std::endl;
		return false;
	}

	return true;
}

int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		std::cout << "Usage: bzip2_decompress <file>" << std::endl;
		return 1;
	}

	std::string file(argv[1]);
	if(!bzip2_decompress(file, file))
		std::cin.get();
	return 0;
}