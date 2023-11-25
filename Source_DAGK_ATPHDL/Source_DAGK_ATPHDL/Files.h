#pragma once
#include"Volume.h"

// define the funtion to list all file from the volume
void listFiles(fstream& file, Volume*& volume, int*& fat, vector<Entry*>& entries);

// define the funtion to check file name(example: text.txt)
bool checkFileName(string fileName, string& name, string& extension);

// define the funtion to set the password for the file in to the volume
void setPasswordFile(fstream& file, Volume*& volume, vector<Entry*>& entries);

// define the funtion to change the password for the file
void changePasswordFile(fstream& file, Volume*& volume, vector<Entry*>& entries);

// define the funtion to check, get file name and get name extension of that file on the path
pair<string, string> readFilePath(string filePath);

// define the funtion to  get name and name extension of the file
pair<string, string> readFile(string fileName);

// define the function to import a file from the outside to the volume
void importFile(fstream& file, Volume*& vol,long sizeVolume, int*& fat, vector<Entry*>& entries);

// define the function to outport a file from the volume to the outside
void outportFile(fstream& file, Volume*& vol,long sizeVolume, int*& fat, vector<Entry*>& entries);

// define the function to delete a file in the volume
void deleteFile(fstream& file, Volume*& vol,long sizeVolume, int*& fat, vector<Entry*>& entries);