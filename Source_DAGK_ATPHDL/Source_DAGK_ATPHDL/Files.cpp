#pragma once
#include"Files.h"

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
		cout << endl << "Not anymore the file in this volume!!!\n\n";
	}
}

// define the funtion to check file name(example: text.txt)
bool checkFileName(string fileName, string& name, string& extension) {
	int pos = fileName.find('.');
	if (pos == string::npos) {
		return false;
	}

	for (int i = 0; i < pos; i++) {
		name.push_back(fileName[i]);
	}

	for (int i = pos + 1; i < fileName.length(); i++) {
		extension.push_back(fileName[i]);
	}

	return true;
}

// define the funtion to set the password for the file in to the volume
void setPasswordFile(fstream& file, Volume*& volume, vector<Entry*>& entries) {
	string filename;
	string name = "";
	string extension = "";


	cout << "\nInput file name to set the password(example: test.txt): ";
	cin >> filename;

	if (!checkFileName(filename, name, extension)) {
		cout << "\nFile name is error!!!\n\n";
		return;
	}
	else {
		int file_pos = -1; // the file's position
		int num_entries = SECTORS_OF_ENTRY * BYTES_OF_SECTOR / sizeof(Entry);
		for (int i = 0; i < num_entries; i++) {
			if (entries[i]->nameEntry == name && entries[i]->nameExtensionEntry == extension) {
				file_pos = i;
				break;
			}
		}

		if (file_pos == -1) {
			cout << "\nThe " << filename << " is not exist in this volume!\n\n";
			return;
		}
		else {
			if (entries[file_pos]->passwordEntry.empty()) {
				cout << "\nInpur the password for this file: ";
				cin >> entries[file_pos]->passwordEntry;

				int entry_pos = (SECTORS_BEFORE_FAT + SECTORS_OF_FAT) * BYTES_OF_SECTOR + file_pos * sizeof(Entry);
				file.seekp(entry_pos, ios::beg);
				file.write((char*)entries[file_pos], sizeof(Entry));
				file.flush();
				cout << "\nThe password had been success!\n\n";
			}
			else {
				cout << "\nThe password has set before!\n\n";
			}

		}
	}
}

// define the funtion to change the password for the file
void changePasswordFile(fstream& file, Volume*& volume, vector<Entry*>& entries) {
	string filename;
	string name = "";
	string extension = "";


	cout << "\nInput the file which need change the password(example: test.txt): ";
	cin >> filename;

	if (!checkFileName(filename, name, extension)) {
		cout << "\nFile name is error!!!\n\n";
		return;
	}

	else {
		int file_pos = -1;
		int num_entries = SECTORS_OF_ENTRY * BYTES_OF_SECTOR / sizeof(Entry);
		for (int i = 0; i < num_entries; i++) {
			if (entries[i]->nameEntry == name && entries[i]->nameExtensionEntry == extension) {
				file_pos = i;
				break;
			}
		}

		if (file_pos == -1) {
			cout << "\nThe " << filename << " is not exist in this volume!\n\n";
			return;
		}
		else {

			if (entries[file_pos]->passwordEntry.empty()) {
				cout << "\nPlease set your password before change it\n\n";
				return;
			}
			else {
				string password;
				cout << "\nInput your old password: ";
				cin >> password;

				if (password != entries[file_pos]->passwordEntry) {
					cout << "\nYour old password is not correct!\n\n";
					return;
				}

				cout << "\nInput your new password: ";
				cin >> entries[file_pos]->passwordEntry;

				int entry_pos = (SECTORS_BEFORE_FAT + SECTORS_OF_FAT) * BYTES_OF_SECTOR + file_pos * sizeof(Entry);
				file.seekp(entry_pos, ios::beg);
				file.write((char*)entries[file_pos], sizeof(Entry));
				file.flush();
				cout << "\n====================Successful ====================!\n\n";
			}

		}
	}
}

// define the funtion to check, get file name and get name extension of that file on the path
pair<string, string> readFilePath(string filePath) {

	string name;
	string extension;
	int length_path = filePath.length();

	int point_pos = filePath.find('.');
	for (int i = point_pos + 1; i < length_path; i++) {
		extension.push_back(filePath[i]);
	}

	for (int i = point_pos - 1; i >= 0; i--) {
		if (filePath[i] == '/' || filePath[i] == '\\') {
			for (int j = i + 1; j < point_pos; j++) {
				name.push_back(filePath[j]);
			}
			break;
		}
	}

	return { name, extension };
}

// define the funtion to  get name and name extension of the file
pair<string, string> readFile(string fileName) {
	string name, extension;

	int pos = fileName.find('.');
	for (int i = 0; i < pos; i++) {
		name.push_back(fileName[i]);
	}

	for (int i = pos + 1; i < fileName.length(); i++) {
		extension.push_back(fileName[i]);
	}

	return { name, extension };
}

// define the function to import a file from the outside to the volume
void importFile(fstream& file, Volume*& vol, long sizeVolume, int*& fat, vector<Entry*>& entries) {

	string filePath;
	fstream fileImport;

	system("cls");
	cout << "\n\t\t========== IMPORT FILE FROM THE OUTSIDE TO THE VOLUME ==========\n";
	cout << "\n\nImport the path of the file to import in the volume(example: D:\\test.txt): ";
	cin >> filePath;
	while (filePath.find(".") == string::npos) {
		system("cls");
		cout << "\n\t\t========== IMPORT FILE FROM THE OUTSIDE TO THE VOLUME ==========";
		cout << "\n\t\t============================= ERROR ============================\n";
		cout << "\nImport the path of the file to import in the volume again(example: D:\\test.txt): ";
		cin >> filePath;
	}

	fileImport.open(filePath, ios::binary | ios::in);

	if (!fileImport.is_open()) {
		cout << "Path: "<< filePath << " is not exit!\n\n";
		return;
	}

	Entry* entryFile = createEntry("", "");
	pair<string, string > res = readFilePath(filePath);
	entryFile->nameEntry = res.first;
	entryFile->nameExtensionEntry = res.second;


	// Read the MyFS.DRS volume's information 
	file.seekg(0, ios::beg);
	file.read((char*)vol, sizeof(Volume));
	file.seekg(SECTORS_BEFORE_FAT * BYTES_OF_SECTOR, ios::beg);
	file.read((char*)fat, sizeVolume);


	int numEntries = SECTORS_OF_ENTRY * BYTES_OF_SECTOR / sizeof(Entry);

	file.seekg((SECTORS_BEFORE_FAT + SECTORS_OF_FAT) * BYTES_OF_SECTOR, ios::beg);
	for (int i = 0; i < numEntries; i++) {
		file.read((char*)entries[i], sizeof(Entry));
	}



	// Check isEmpty for a entry
	int entryPos = -1;
	for (int i = 0; i < numEntries; i++) {
		if (entries[i]->nameEntry.empty()) {
			entryPos = i;
			break;
		}
	}

	if (entryPos == -1) {
		cout << "\nThe volume don't have enough empty entry!!!\n\n";
		return;
	}

	// Check the fat have enough cluster empty
	int clusterPos = -1;
	for (int i = 0; i < sizeVolume; i++) {
		if (fat[i] == CLUSTER_EMPTY) {
			clusterPos = i;
			break;
		}
	}

	if (clusterPos == -1) {
		cout << "\nThe Volume don't have place to save this file!!!\n\n";
		return;
	}


	// Write the size information of the file
	fileImport.seekg(0, ios::end);
	entryFile->size = fileImport.tellg();

	// Update pointer at begin place of the file
	fileImport.seekg(0, ios::beg);

	// Writes the first location of the file in the Data area to the empty cluster in the volume
	entryFile->cluster_begin = clusterPos;

	while (true) {

		// Calculate the number of system bytes
		int systemBytes = (SECTORS_BEFORE_FAT + SECTORS_OF_FAT + SECTORS_OF_ENTRY) * BYTES_OF_SECTOR;


		// Read each sector then write to file
		for (int i = 0; i < SECTORS_OF_CLUSTER; i++) {
			char* sector = new char[BYTES_OF_SECTOR];
			fileImport.read(sector, BYTES_OF_SECTOR);

			int writePos = systemBytes + (clusterPos * SECTORS_OF_CLUSTER + i) * BYTES_OF_SECTOR;

			//.seekp(write_pos, ios::beg);
			//file.write(sector, BYTES_OF_SECTOR);
			writeSector(file, sector, writePos);
		}


		// Check if the file has been completely written, then update the fat table and exit the loop
		if (fileImport.eof()) {
			fat[clusterPos] = CLUSTER_NOT_EMPTY;
			break;
		}

		// If the file has not been read yet, continue to find the next empty cluster by increasing the current cluster position by 1 unit 
		int currClusterPos = clusterPos + 1;
		while (currClusterPos < sizeVolume) {
			if (fat[currClusterPos] == CLUSTER_EMPTY) {
				// Update fat table value at clusterPos location
				fat[clusterPos] = currClusterPos;

				// Update the next free cluster location to continue writing file contents
				clusterPos = currClusterPos;

				break;
			}

			currClusterPos++;
		}

		if (currClusterPos == sizeVolume) {
			cout << "\n========== The MyFS.DRS Volume is full!!! ==========\n\n";
			return;
		}
	}


	// Write file information to an empty entry in the volume
	entries[entryPos] = entryFile;
	int emptyEntryPos = (SECTORS_BEFORE_FAT + SECTORS_OF_FAT) * BYTES_OF_SECTOR + entryPos * sizeof(Entry);
	file.seekp(emptyEntryPos, ios::beg);
	file.write((char*)entries[entryPos], sizeof(Entry));

	// Update fat into file
	file.seekp(SECTORS_BEFORE_FAT * BYTES_OF_SECTOR, ios::beg);
	file.write((char*)fat, sizeVolume);


	cout << "\n==================== Import Successfully! ====================\n\n";
}

// define the function to outport a file from the volume to the outside
void outportFile(fstream& file, Volume*& vol,long sizzeVolume, int*& fat, vector<Entry*>& entries) {
	string fileName;

	system("cls");
	cout << "\n\t\t========== OUTPORT FILE FROM THE OUTSIDE TO THE VOLUME ==========\n";
	cout << "\nInput the file name to outport to the outside(example: test.txt): ";
	cin >> fileName;

	while (fileName.find(".") == string::npos) {
		system("cls");
		cout << "\n\t\t========== OUTPORT FILE FROM THE OUTSIDE TO THE VOLUME ==========";
		cout << "\n\t\t============================= ERROR =============================\n";
		cout << "Input the file name to outport to the outside again(example: test.txt): ";
		cin >> fileName;
	}
	
	// Read information of volume MyFS.DRS
	file.seekg(0, ios::beg);
	file.read((char*)vol, sizeof(Volume));

	file.seekg(SECTORS_BEFORE_FAT * BYTES_OF_SECTOR, ios::beg);
	file.read((char*)fat, sizzeVolume);

	int numEntries = SECTORS_OF_ENTRY * BYTES_OF_SECTOR / sizeof(Entry);
	file.seekg((SECTORS_BEFORE_FAT + SECTORS_OF_FAT) * BYTES_OF_SECTOR, ios::beg);
	for (int i = 0; i < numEntries; i++) {
		file.read((char*)entries[i], sizeof(Entry));
	}

	// Based on the file name and file extension to determine the file location in the RDET table
	pair<string, string> res = readFile(fileName);
	int entryPosFile = -1;
	for (int i = 0; i < numEntries; i++) {
		if (entries[i]->nameEntry == res.first && entries[i]->nameExtensionEntry == res.second) {
			entryPosFile = i;
			break;
		}
	}


	// If this file does not exist in RDET, an error will be printed
	if (entryPosFile == -1) {
		cout << "\nThe " << fileName << " is not exist in this volume\n\n";
		return;
	}

	ofstream outfile;
	outfile.open(fileName, ios::binary);

	int clusterPos = entries[entryPosFile]->cluster_begin;
	int sizeFile = entries[entryPosFile]->size;
	while (true) {
		int systemBytes = (SECTORS_BEFORE_FAT + SECTORS_OF_FAT + SECTORS_OF_ENTRY) * BYTES_OF_SECTOR;

		for (int i = 0; i < SECTORS_OF_CLUSTER; i++) {
			char* sector = new char[BYTES_OF_SECTOR];
			int readPos = systemBytes + (clusterPos * SECTORS_OF_CLUSTER + i) * BYTES_OF_SECTOR;

			//file.seekg(read_pos, ios::beg);
			//file.read(sector, BYTES_OF_SECTOR);

			sector = readSector(file, readPos);
			// If the size is larger than the number of bytes in 1 sector, then write data in 1 sector
			if (sizeFile > BYTES_OF_SECTOR) {
				outfile.write(sector, BYTES_OF_SECTOR);
				sizeFile -= BYTES_OF_SECTOR;
			}
			// Size is smaller than the number of bytes in 1 sector, then write each byte and exit the loop and end
			else {
				outfile.write(sector, sizeFile);
				sizeFile = 0;
				break;
			}
		}

		if (sizeFile == 0) {
			break;
		}

		// If you haven't finished recording, find the next cluster in linked list form to continue recording
		clusterPos = fat[clusterPos];
	}

	cout << "\nThe " << fileName << " file is outported the outside!\n\n";
}

// define the function to delete a file in the volume
void deleteFile(fstream& file, Volume*& vol, long sizeVolume, int*& fat, vector<Entry*>& entries) {
	string fileName;

	system("cls"); 
	cout << "\n\t\t=============== DELETE THE FILE OF THE VOLUME ===============\n";
	cout << "Import the file name to delete(example: test.txt): ";
	cin >> fileName;

	while (fileName.find(".") == string::npos) {
		system("cls");
		cout << "\n\t\t=============== DELETE THE FILE OF THE VOLUME ===============";
		cout << "\n\t\t=========================== ERROR ===========================\n";
		cout << "Import the file name to delete again(example: test.txt): ";
		cin >> fileName;
	}

	// Read information of volume MyFS.DRS
	file.seekg(0, ios::beg);
	file.read((char*)vol, sizeof(Volume));
	file.seekg(SECTORS_BEFORE_FAT * BYTES_OF_SECTOR, ios::beg);
	file.read((char*)fat, sizeVolume);

	int numEntries = SECTORS_OF_ENTRY * BYTES_OF_SECTOR / sizeof(Entry);
	file.seekg((SECTORS_BEFORE_FAT + SECTORS_OF_FAT) * BYTES_OF_SECTOR, ios::beg);
	for (int i = 0; i < numEntries; i++) {
		file.read((char*)entries[i], sizeof(Entry));
	}


	// Based on the file name and file extension to determine the file location in the RDET table
	pair<string, string> res = readFile(fileName);
	int entryPosFile = -1;

	for (int i = 0; i < numEntries; i++) {
		if (entries[i]->nameEntry == res.first && entries[i]->nameExtensionEntry == res.second) {
			entryPosFile = i;
			break;
		}
	}

	// If this file does not exist in RDET, an error will be printed
	if (entryPosFile == -1) {
		cout << "\nThe " << fileName << " is not exist in the volume\n\n";
		return;
	}


	int cluster = entries[entryPosFile]->cluster_begin;
	while (cluster != CLUSTER_EMPTY) {
		if (fat[cluster] == CLUSTER_NOT_EMPTY)
		{
			fat[cluster] = CLUSTER_EMPTY;
			break;
		}
		int nextCluster = fat[cluster];
		fat[cluster] = CLUSTER_EMPTY;
		cluster = nextCluster;
	}


	// Write empty entries to the file
	int entryPos = (SECTORS_BEFORE_FAT + SECTORS_OF_FAT) * BYTES_OF_SECTOR + entryPosFile * sizeof(Entry);
	Entry* delEntry = createEntry("", "");
	entries[entryPosFile] = delEntry;
	file.seekp(entryPos, ios::beg);
	file.write((char*)delEntry, sizeof(Entry));

	// Update the fat table
	file.seekp(SECTORS_BEFORE_FAT * BYTES_OF_SECTOR, ios::beg);
	file.write((char*)fat, sizeVolume);

	cout << "\nThe " << fileName << " was deleted!\n\n";
}