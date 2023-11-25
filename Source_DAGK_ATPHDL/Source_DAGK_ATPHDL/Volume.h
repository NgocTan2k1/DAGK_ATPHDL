#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<fstream>

using namespace std;

// define information for volume
#define BYTES_OF_SECTOR 512
#define SECTORS_OF_CLUSTER 4
#define SECTORS_OF_FAT 5
#define SECTORS_BEFORE_FAT 1
#define SECTORS_OF_ENTRY 2
#define CLUSTER_EMPTY -1
#define CLUSTER_NOT_EMPTY -2
//#define CLUSTERS_OF_VOLUME 2000


// define the Volume struct which contain some bootsector infomation
struct Volume
{
	string nameVolume;
	string nameExtensionVolume;
	string passwordVolume;
	unsigned int bytes_of_sector;
	unsigned int sectors_of_cluster;
	unsigned int sectors_of_entry;
	unsigned int sectors_of_fat;
	unsigned int clusters_of_volume;
};

// define the main Entry struct
struct Entry
{
	string nameEntry;
	string nameExtensionEntry;
	string passwordEntry;
	int size;
	int cluster_begin;
};

// define some funtions

// define the function generates information about a volume
Volume* createVolume(string name, string extension, long sizeVolume);

// define the function generates information about the main Entry
Entry* createEntry(string name, string extension);

// define the function to format the MyFS.DRS volume.
void formatVolumeDRS(string name, string extension, Volume*& volume, long sizeVolume, int*& fat, vector<Entry*>& entries);

// define the function to read the sector
char* readSector(fstream& file, int pos);

// define the function to write the sector
void writeSector(fstream& file, char* sector, int pos);

// define the funtion to set the password for the volume
void setPasswordVolume(fstream& file, Volume*& volume);

// define the funtion to change the password for the volume
void changePasswordVolume(fstream& file, Volume*& volume);

// define the funtion to check the password for the volume
int checkPasswordVolume(Volume* volume);