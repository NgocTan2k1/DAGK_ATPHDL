#pragma once
#include"Volume.h"

// define the funtion to set the password for the volume
void setPasswordVolume(fstream& file, Volume*& volume);

// define the funtion to change the password for the volume
void changePasswordVolume(fstream& file, Volume*& volume);

// define the funtion to check the password for the volume
bool checkPasswordVolume(Volume* volume);

// define the funtion to check file name(example: text.txt)
bool checkFileName(string filename, string& name, string& extension);

// define the funtion to set the password for the file in to the volume
void setPasswordFile(fstream& file, Volume*& volume, vector<Entry*>& entries);

// define the funtion to change the password for the file
void changePasswordFile(fstream& file, Volume*& volume, vector<Entry*>& entries);