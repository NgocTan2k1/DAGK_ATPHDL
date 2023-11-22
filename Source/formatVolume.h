#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// Thiết lập các thông số để định dạng volume
#define BYTES_PER_SECTOR 512
#define SECTORS_PER_CLUSTER 2
#define SECTORS_OF_FAT 5
#define SECTORS_OF_ENTRY 2
#define CLUSTERS_OF_VOLUME 2000
#define SECTORS_BEFORE_FAT 1
#define CLUSTER_EMPTY -1
#define CLUSTER_NOT_EMPTY -2

// Tạo struct volume để chứa các thông tin bootsector của 1 volume
struct Volume {
	string name;
	string extension;
	string password;
	unsigned int bytes_per_sector;
	unsigned int sectors_per_cluster;
	unsigned int sectors_of_entry;
	unsigned int sectors_of_fat;
	unsigned int clusters_of_volume;
};

// Tạo struct Entry chính
struct Entry {
	string name;
	string extension;
	string password;
	int size;
	int cluster_begin;
};


// Tạo ra đối tượng thông tin của 1 volume
Volume* createVolumeInfo(string name, string extension);

// Tạo ra đối tượng thông tin của 1 entry chính
Entry* createEntry(string name, string extension);

// Hàm định dạng volume MyFS.DRS 
void formatVolume(string name, string extension, Volume*& vol, int*& fat, vector<Entry*>& entries);

// Hàm đọc sector 
char* readSector(fstream& file, int pos);

// Hàm viết  sector 
void writeSector(fstream& file,char*sector, int pos);
