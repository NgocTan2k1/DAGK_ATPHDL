#include "password.h"


// Hàm thiết lập mật khẩu cho volume
void setPasswordVolume(fstream& file, Volume*& vol) {
	if (vol->password.empty()) {
		cout << "\nNhap mat khau thiet lap volume: ";
		cin >> vol->password;

		file.seekp(0, ios::beg);
		file.write((char*)vol, sizeof(Volume));
		file.flush();
		cout << "\nMat khau thiet lap thanh cong!\n\n";
	}

	else {
		cout << "\nMat khau da duoc thiet lap!\n\n";
	}
}


// Hàm thay đổi mật khảu volume
void changePasswordVolume(fstream& file, Volume*& vol) {
	if (vol->password.empty()) {
		cout << "\nMat khau chua duoc thiet lap.\nXin vui long thiet lap mat khau truoc!\n\n";
		return;
	}
	else {
		string password;
		cout << "\nNhap mat khau hien tai: ";
		cin >> password;

		if (password != vol->password) {
			cout << "\nMat khau khong khop voi mat khau hien tai!\n\n";
			return;
		}

		cout << "\nNhap mat khau moi: ";
		cin >> vol->password;

		file.seekp(0, ios::beg);
		file.write((char*)vol, sizeof(Volume));
		file.flush();
		cout << "\nMat khau da duoc thay doi!\n\n";
	}
}

// Hàm kiểm tra mật khẩu volume
bool checkPasswordVolume(Volume* vol) {
	if (vol->password.empty()) {
		return true;
	}
	else {
		string password;
		cout << "\nNhap mat khau MyFS.DRS: ";
		cin >> password;

		if (password == vol->password) {
			return true;
		}

		return false;
	}
}

// Kiểm tra tên file có hợp lệ hay không(hợp lệ khi có cấu trúc như name.txt)
bool checkFileName(string filename, string & name, string & extension) {
	int pos = filename.find('.');
	if (pos == string::npos) {
		return false;
	}

	for (int i = 0; i < pos; i++) {
		name.push_back(filename[i]);
	}

	for (int i = pos + 1; i < filename.length(); i++) {
		extension.push_back(filename[i]);
	}

	return true;
}


// Hàm thiết lập mật khẩu cho tập tin trong volume
void setPasswordFile(fstream& file, Volume*& vol, vector<Entry*>& entries) {
	string filename;
	string name = "";
	string extension = "";


	cout << "\nNhap ten file can thiet lap mat khau (Ex:test.txt) : ";
	cin >> filename;

	if (!checkFileName(filename, name, extension)) {
		cout << "\nTen file khong hop le!!!\n";
		return;
	}

	else {
		int file_pos = -1; // vị trí của tập tin cần tìm
		int num_entries = SECTORS_OF_ENTRY * BYTES_PER_SECTOR / sizeof(Entry);
		for (int i = 0; i < num_entries; i++) {
			if (entries[i]->name == name && entries[i]->extension == extension) {
				file_pos = i;
				break;
			}
		}

		if (file_pos == -1) {
			cout << "\nKhong ton tai tap tin " << filename << " trong volume!\n\n";
			return;
		}

		else {
			if (entries[file_pos]->password.empty()) {
				cout << "\nNhap mat khau thiet lap cho tap tin: ";
				cin >> entries[file_pos]->password;

				int entry_pos = (SECTORS_BEFORE_FAT+SECTORS_OF_FAT)*BYTES_PER_SECTOR + file_pos * sizeof(Entry);
				file.seekp(entry_pos, ios::beg);
				file.write((char*)entries[file_pos], sizeof(Entry));
				file.flush();
				cout << "\nMat khau thiet lap thanh cong!\n\n";
			}

			else {
				cout << "\nMat khau da duoc thiet lap!\n\n";
			}

		}
	}
}

// Hàm thay đổi mật khẩu tập tin trong volume
void changePasswordFile(fstream& file, Volume*& vol, vector<Entry*>& entries) {
	string filename;
	string name = "";
	string extension = "";


	cout << "\nNhap ten file can thay doi lap mat khau (vd:test.txt) : ";
	cin >> filename;

	if (!checkFileName(filename, name, extension)) {
		cout << "\nTen file khong hop le!!!\n";
		return;
	}

	else {
		int file_pos = -1; // vị trí của tập tin cần tìm
		int num_entries = SECTORS_OF_ENTRY * BYTES_PER_SECTOR / sizeof(Entry);
		for (int i = 0; i < num_entries; i++) {
			if (entries[i]->name == name && entries[i]->extension == extension) {
				file_pos = i;
				break;
			}
		}

		if (file_pos == -1) {
			cout << "\nKhong ton tai tap tin " << filename << " trong volume!\n\n";
			return;
		}

		else {

			if (entries[file_pos]->password.empty()) {
				cout << "\nMat khau chua duoc thiet lap!!!\nXin vui long thiet lap mat khau truoc!\n\n";
				return;
			}
			else {
				string password;
				cout << "\nNhap mat khau hien tai: ";
				cin >> password;

				if (password != entries[file_pos]->password) {
					cout << "\nMat khau khong khop voi mat khau hien tai!\n\n";
					return;
				}

				cout << "\nNhap mat khau moi: ";
				cin >> entries[file_pos]->password;

				int entry_pos = (SECTORS_BEFORE_FAT + SECTORS_OF_FAT) * BYTES_PER_SECTOR + file_pos * sizeof(Entry);
				file.seekp(entry_pos, ios::beg);
				file.write((char*)entries[file_pos], sizeof(Entry));
				file.flush();
				cout << "\nMat khau da duoc thay doi thanh cong!\n\n";
			}

		}
	}
}






