#pragma once
#include <Windows.h>
#include <vector>

using namespace std;
#define DELIM "="

struct config {
	char ESPURL[1024];
	char PostURL[1024];
};

struct config get_config(char* filename) {
	struct config configstruct;
	FILE *file;
	fopen_s(&file,filename, "r");

	if (file != nullptr)
	{
		char line[1024];
		int i = 0;

		while (fgets(line, sizeof(line), file) != nullptr) {

			char *cfline;
			cfline = strstr((char *)line, DELIM);
			cfline = cfline + strlen(DELIM);

			if (i == 0) {
				memcpy(configstruct.ESPURL, cfline + '\0', strlen(cfline) + 1);
			}
			else if (i == 1) {
				memcpy(configstruct.PostURL, cfline + '\0', strlen(cfline) + 1);
			}
			i++;
		} // End while
		fclose(file);
	} // End if file

	return  configstruct;

}