#include"Volume.h"

// define the function generates information about a volume
Volume* createVolume(string name, string extension) {
	Volume* volume = new Volume;

	volume->nameVolume = name;
	volume->nameExtensionVolume = extension;
	volume->passwordVolume = "";
	volume->sectors_of_entry = SECTORS_OF_ENTRY;
	volume->bytes_of_sector = BYTES_OF_SECTOR;
	volume->sectors_of_cluster = SECTORS_OF_CLUSTER;
	volume->sectors_of_fat = SECTORS_OF_FAT;
	volume->clusters_of_volume = CLUSTERS_OF_VOLUME;

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
void formatVolumeDRS(string name, string extension, Volume*& volume, int*& fat, vector<Entry*>& entries) {
	ofstream file;
	
	file.open("MyFS.DRS", ios::binary);

	volume = createVolume(name, extension);

	// Write the volume infomation into the MyFS.DRS file 
	file.seekp(0, ios::beg);
	file.write((char*)volume, sizeof(Volume));

	fat = new int[CLUSTERS_OF_VOLUME];
	
	// create ClusterManagement from Cluster which we define before
	for (int i = 0; i < CLUSTERS_OF_VOLUME; i++) {
		fat[i] = CLUSTER_EMPTY;
	}

	// Write the ClusterManagement(FAT) into the MyFS.DRS file 
	file.seekp(SECTORS_BEFORE_FAT * BYTES_OF_SECTOR, ios::beg);
	file.write((char*)fat, CLUSTERS_OF_VOLUME);

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
	for (int i = 0; i < CLUSTERS_OF_VOLUME; i++) {
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