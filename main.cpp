#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <dirent.h>
#include <zip.h>

int startsWith(std::string file, std::string compare) {
	if (!file.compare(0,10,compare)) {
		return 1;
	}
	else {
		return 0;
	}
}

std::vector<std::string> directoryGrab() {
	DIR *dir;
	struct dirent *ent;
	
	std::vector<std::string> directoryList;
	std::string filePrefix = "esea_match";
	
	if ((dir = opendir ("C:\\Users\\distant\\Downloads")) != NULL) {
	/* print all the files and directories within directory */
	while ((ent = readdir (dir)) != NULL) {
		if (startsWith(ent->d_name, filePrefix)) {
			directoryList.push_back(ent->d_name);
		}
	}
	closedir (dir);
	} else {
	  /* could not open directory */
	  perror ("");
	  EXIT_FAILURE;
	}
	
	return directoryList;
}

int main() {

	for (int i=0; i < directoryGrab().size(); i++) {
		std::cout << directoryGrab()[i] << std::endl;
	}

	//Open the ZIP archive
	int err = 0;
	std::string demo = "C:\\Users\\distant\\Downloads\\" + directoryGrab()[6];
    zip *z = zip_open(demo.c_str(), 0, &err);

    //Get file name
	const char* demoName = zip_get_name(z, 0,0);
	
	std::cout << demoName << std::endl;
	
	//Search for the file of given name
    struct zip_stat st;
    zip_stat_init(&st);
    zip_stat(z, demoName, 0, &st);

    //Alloc memory for its uncompressed contents
    char *contents = new char[st.size];

    //Read the compressed file
    zip_file *f = zip_fopen(z, demoName, 0);
    zip_fread(f, contents, st.size);
	
	if(!std::ofstream(demoName).write(contents, st.size))
	{
		std::cerr << "Error writing file" << '\n';
		return EXIT_FAILURE;
	}
	
    zip_fclose(f);

    //And close the archive
    zip_close(z);
}
