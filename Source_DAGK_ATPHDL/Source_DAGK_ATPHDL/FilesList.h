#pragma once
#include"Volume.h"

// define the funtion to list all file from the volume
void listFiles(fstream& file, Volume*& volume, int*& fat, vector<Entry*>& entries);