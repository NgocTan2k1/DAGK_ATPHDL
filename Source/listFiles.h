#pragma once
#include "formatVolume.h"


// Hàm liệt kê các tập tin có trong volume
void listFiles(fstream& file, Volume*& vol, int*& fat, vector<Entry*>& entries);