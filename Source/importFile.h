#pragma once
#include "listFiles.h"



// Kiểm tra và tách ra tên cùng phần đuôi của đường dẫn file
pair<string, string> readFilePath(string file_path);

// Hàm import file từ bên ngoài vào trong volume
void importFile(fstream& file, Volume*& vol, int*& fat, vector<Entry*>& entries);
