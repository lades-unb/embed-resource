#include <fstream>
#include <Poco/File.h>
#include <Poco/Path.h>

using namespace std;
using Poco::File;
using Poco::Path;

void replace(std::string& str, string toSearch, string toReplace) {
	size_t pos;
	while ((pos = str.find(toSearch)) != string::npos) {
		str.replace(pos, 1, toReplace);
		pos++;
	}
}

int main(int argc, char** argv)
{
    if (argc < 3) {
        fprintf(stderr, "USAGE: %s {sym} {rsrc}\n\n"
                        "  Creates {sym}.c from the contents of {rsrc}\n",
                argv[0]);
        return EXIT_FAILURE;
    }

	Path dstPath{argv[1]};
    File dstFile(dstPath);
	Path srcPath{argv[2]};
    File srcFile(srcPath);

	string sym = srcPath.getFileName();
	replace(sym, ".", "_");
	replace(sym, "-", "_");

	File(dstPath.parent()).createDirectories();

    ofstream ofs{dstFile.path()};
    ifstream ifs{srcFile.path()};

    ofs << "#include <stdlib.h>" << endl;
    ofs << "extern const char _resource_" << sym << "[] = {" << endl;

    size_t lineCount = 0;
    while (true)
    {
        char c;
        ifs.get(c);

		if (ifs.eof())
			break;

        ofs << "0x" << hex << (unsigned int)c << ", ";
        if (++lineCount == 10) {
            ofs << endl;
            lineCount = 0;
        }
    }


    ofs << "};" << endl;
    ofs << "extern const size_t _resource_" << sym << "_len = sizeof(_resource_" << sym << ");" << endl;

    return EXIT_SUCCESS;
}
