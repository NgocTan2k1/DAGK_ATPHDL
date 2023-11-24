#pragma once
#include"FilesList.h"

// define the funtion to list all file from the volume
void listFiles(fstream& file, Volume*& volume, int*& fat, vector<Entry*>& entries) {

	int num_entries = SECTORS_OF_ENTRY * BYTES_OF_SECTOR / sizeof(Entry);
	bool no_file = true;

	// Browse the directory table(FilesManagement - Entries) to print out existing files
	for (int i = 0; i < num_entries; i++) {
		if (entries[i]->nameEntry.empty()) continue;

		no_file = false;
		cout << entries[i]->nameEntry + "." + entries[i]->nameExtensionEntry << endl;
	}

	if (no_file) {
		cout << endl << "Not anymore the file in this volume!!!";
	}
}