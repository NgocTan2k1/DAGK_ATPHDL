#pragma once
#include "importfile.h"

// Hàm tách tên và đuôi file từ tên file
pair<string, string> readFile(string filename);

// Hàm outport file từu volume ra bên ngoài
void outportFile(fstream& file, Volume*& vol, int*& fat, vector<Entry*>& entries);
