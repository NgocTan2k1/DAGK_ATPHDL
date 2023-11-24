#pragma once
#include "formatVolume.h"

// Hàm thiết lập mật khẩu cho volume
void setPasswordVolume(fstream& file, Volume*& vol);

// Hàm thay đổi mật khảu volume
void changePasswordVolume(fstream& file, Volume*& vol);

// Hàm kiểm tra mật khẩu volume
bool checkPasswordVolume(Volume* vol);

// Kiểm tra tên file có hợp lệ hay không (hợp lệ khi có cấu trúc như name.txt)
bool checkFileName(string filename, string& name, string& extension);


// Hàm thiết lập mật khẩu cho tập tin trong volume
void setPasswordFile(fstream& file, Volume*& vol, vector<Entry*>& entries);

// Hàm thay đổi mật khẩu tập tin trong volume
void changePasswordFile(fstream& file, Volume*& vol, vector<Entry*>& entries);
