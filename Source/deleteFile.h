#pragma once
#include "outportFile.h"

// H�m x�a file trong volume
void deleteFile(fstream& file, Volume*& vol, int*& fat, vector<Entry*>& entries);