#include "formatVolume.h"

// Tạo ra đối tượng thông tin của 1 volume
Volume* createVolumeInfo(string name, string extension) {
	Volume* volume = new Volume;
	volume->name = name;
	volume->extension = extension;
	volume->password = "";
	volume->sectors_of_entry = SECTORS_OF_ENTRY;
	volume->bytes_per_sector = BYTES_PER_SECTOR;
	volume->sectors_per_cluster = SECTORS_PER_CLUSTER;
	volume->sectors_of_fat = SECTORS_OF_FAT;
	volume->clusters_of_volume = CLUSTERS_OF_VOLUME;
	return volume;
}

// Tạo ra đối tượng thông tin của 1 entry chính
Entry* createEntry(string name, string extension) {
	Entry* entry = new Entry;
	entry->name = name;
	entry->extension = extension;
	entry->password = "";
	entry->size = 0;
	entry->cluster_begin = CLUSTER_EMPTY;
	return entry;
}


// Hàm định dạng volume MyFS.DRS
void formatVolume( string name, string extension, Volume*& vol, int*& fat, vector<Entry*>& entries) {
	ofstream file;
	file.open("MyFS.DRS", ios::binary);

	vol = createVolumeInfo(name, extension);


	// Viết thông tin volume vào file
	file.seekp(0,ios::beg);
	file.write((char*)vol, sizeof(Volume));

	
	fat = new int[CLUSTERS_OF_VOLUME];
	for (int i = 0; i < CLUSTERS_OF_VOLUME; i++) {
		fat[i] = CLUSTER_EMPTY;
	}

	// Viết thông tin bảng fat vào file 
	file.seekp(SECTORS_BEFORE_FAT*BYTES_PER_SECTOR, ios::beg);
	file.write((char*)fat, CLUSTERS_OF_VOLUME);


	// Viết thông tin bảng entry vào file 
	int num_entries = SECTORS_OF_ENTRY * BYTES_PER_SECTOR / sizeof(Entry);
	for (int i = 0; i < num_entries; i++) {
		Entry* entry = createEntry("", "");
		entries.push_back(entry);
		file.seekp((SECTORS_BEFORE_FAT + SECTORS_OF_FAT )* BYTES_PER_SECTOR + (static_cast<unsigned __int64>(i) + 1) * sizeof(Entry), ios::beg);
		file.write((char*)entry, sizeof(Entry));
	}

	// Viết vùng data vào file
	int system_bytes =  (SECTORS_BEFORE_FAT+ SECTORS_OF_FAT + SECTORS_OF_ENTRY) * BYTES_PER_SECTOR;
	for (int i = 0; i < CLUSTERS_OF_VOLUME; i++) {
		for (int j = 0; j < SECTORS_PER_CLUSTER; j++) {
			int* block = new int[BYTES_PER_SECTOR];
			int pos = system_bytes + (i * SECTORS_PER_CLUSTER + j) * BYTES_PER_SECTOR;
			file.seekp(pos, ios::beg);
			file.write((char*)block, BYTES_PER_SECTOR);
		}
	}

	file.close();
}
// Hàm đọc sector 
char* readSector(fstream& file, int pos)
{
	char* sector = new char[BYTES_PER_SECTOR];
	file.seekg(pos, ios::beg);
	file.read(sector, BYTES_PER_SECTOR);
	return sector;
}

// Hàm viết  sector 
void writeSector(fstream& file, char* sector, int pos)
{ 
	file.seekp(pos, ios::beg);
	file.write(sector, BYTES_PER_SECTOR);
}