#include"Volume.h"

// define the function generates information about a volume
Volume* createVolume(string name, string extension, long sizeVolume) {
	Volume* volume = new Volume;

	volume->nameVolume = name;
	volume->nameExtensionVolume = extension;
	volume->passwordVolume = "";
	volume->sectors_of_entry = SECTORS_OF_ENTRY;
	volume->bytes_of_sector = BYTES_OF_SECTOR;
	volume->sectors_of_cluster = SECTORS_OF_CLUSTER;
	volume->sectors_of_fat = SECTORS_OF_FAT;
	volume->clusters_of_volume = sizeVolume;

	return volume;
}

// define the function generates information about the main Entry
Entry* createEntry(string name, string extension) {
	Entry* entry = new Entry;

	entry->nameEntry = name;
	entry->nameExtensionEntry = extension;
	entry->passwordEntry = "";
	entry->size = 0;
	entry->cluster_begin = CLUSTER_EMPTY;

	return entry;
}

// define the function to format the MyFS.DRS volume.
void formatVolumeDRS(string name, string extension, Volume*& volume, long sizeVolume, int*& fat, vector<Entry*>& entries) {
	ofstream file;
	
	file.open("MyFS.DRS", ios::binary);

	volume = createVolume(name, extension, sizeVolume);

	// Write the volume infomation into the MyFS.DRS file 
	file.seekp(0, ios::beg);
	file.write((char*)volume, sizeof(Volume));

	fat = new int[sizeVolume];
	
	// create ClusterManagement from Cluster which we define before
	for (int i = 0; i < sizeVolume; i++) {
		fat[i] = CLUSTER_EMPTY;
	}

	// Write the ClusterManagement(FAT) into the MyFS.DRS file 
	file.seekp(SECTORS_BEFORE_FAT * BYTES_OF_SECTOR, ios::beg);
	file.write((char*)fat, sizeVolume);

	// Write the entry into the MyFS.DRS file
	int num_entries = SECTORS_OF_ENTRY * BYTES_OF_SECTOR / sizeof(Entry);

	for (int i = 0; i < num_entries; i++) {
		Entry* entry = createEntry("", "");

		entries.push_back(entry);

		file.seekp((SECTORS_BEFORE_FAT + SECTORS_OF_FAT) * BYTES_OF_SECTOR + (static_cast<unsigned __int64>(i) + 1) * sizeof(Entry), ios::beg);
		file.write((char*)entry, sizeof(Entry));
	}

	// Write the data area into the MyFS.DRS file 
	int system_bytes = (SECTORS_BEFORE_FAT + SECTORS_OF_FAT + SECTORS_OF_ENTRY) * BYTES_OF_SECTOR;
	for (int i = 0; i < sizeVolume; i++) {
		for (int j = 0; j < SECTORS_OF_CLUSTER; j++) {
			int* block = new int[BYTES_OF_SECTOR];
			int pos = system_bytes + (i * SECTORS_OF_CLUSTER + j) * BYTES_OF_SECTOR;
			file.seekp(pos, ios::beg);
			file.write((char*)block, BYTES_OF_SECTOR);
		}
	}
	file.close();
}

// define the function to read the sector
char* readSector(fstream& file, int pos) {
	char* sector = new char[BYTES_OF_SECTOR];

	file.seekg(pos, ios::beg);

	file.read(sector, BYTES_OF_SECTOR);

	return sector;
}

// define the function to write the sector
void writeSector(fstream& file, char* sector, int pos) {

	file.seekp(pos, ios::beg);

	file.write(sector, BYTES_OF_SECTOR);
}

// define the funtion to set the password for the volume
void setPasswordVolume(fstream& file, Volume*& volume) {
	if (volume->passwordVolume.empty()) {

		cout << "\nInput the Volume's password: ";
		cin >> volume->passwordVolume;

		file.seekp(0, ios::beg);
		file.write((char*)volume, sizeof(Volume));
		file.flush();
		cout << "\nSetting the password is successful!\n\n";
	}
	else {
		cout << "\nThe volume had the password!\n\n";
	}
}

// define the funtion to change the password for the volume
void changePasswordVolume(fstream& file, Volume*& volume) {
	if (volume->passwordVolume.empty()) {
		cout << "\nPlease set password for the volume before you have changed it\n\n";
		return;
	}
	else {
		string password;
		cout << "\nInput your old password: ";
		cin >> password;

		if (password != volume->passwordVolume) {
			cout << "\nYour old password is incorrect!\n\n";
			return;
		}

		cout << "\nInput your new password: ";
		cin >> volume->passwordVolume;

		file.seekp(0, ios::beg);
		file.write((char*)volume, sizeof(Volume));
		file.flush();
		cout << "\nYour password had changed!\n\n";
		cout << "\n\t===================================== DONE ==================================\n\n";

	}
}

// define the funtion to check the password for the volume
int checkPasswordVolume(Volume* volume) {
	if (volume->passwordVolume.empty()) {
		return 0;
	} else {
		string password;
		cout << "\nInput the password for MyFS.DRS file: ";
		cin >> password;

		if (password == volume->passwordVolume) {
			return 1;
		}
		return 2;
	}
}