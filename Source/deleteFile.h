#pragma once
#include "outportFile.h"

// Hàm xóa file trong volume
void deleteFile(fstream& file, Volume*& vol, int*& fat, vector<Entry*>& entries);