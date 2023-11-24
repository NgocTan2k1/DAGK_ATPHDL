#include"Password.h"

// define the funtion to set the password for the volume
void setPasswordVolume(fstream& file, Volume*& volume) {
	if (volume->passwordVolume.empty()) {

		cout << "\nInput the Volume's password: ";
		cin >> volume->passwordVolume;

		file.seekp(0, ios::beg);
		file.write((char*)volume, sizeof(Volume));
		file.flush();
		cout << "\nSetting the password is successful!";
	} else {
		cout << "\nThe volume had the password!";
	}
}

// define the funtion to change the password for the volume
void changePasswordVolume(fstream& file, Volume*& volume) {
	if (volume->passwordVolume.empty()) {
		cout << "\nPlease set password for the volume before you have changed it";
		return;
	} else {
		string password;
		cout << "\nInput your password: ";
		cin >> password;

		if (password != volume->passwordVolume) {
			cout << "\nYour password is incorrect!";
			return;
		}

		cout << "\nInput your new password: ";
		cin >> volume->passwordVolume;

		file.seekp(0, ios::beg);
		file.write((char*)volume, sizeof(Volume));
		file.flush();
		cout << "\nYour password had changed!\n\n";
	}
}

// define the funtion to check the password for the volume
bool checkPasswordVolume(Volume* volume) {
	if (volume->passwordVolume.empty()) {
		return true;
	} else {
		string password;
		cout << "\nInput the password for MyFS.DRS file: ";
		cin >> password;

		if (password == volume->passwordVolume) {
			return true;
		}

		return false;
	}
}

// define the funtion to check file name(example: text.txt)
bool checkFileName(string filename, string& name, string& extension) {
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

// define the funtion to set the password for the file in to the volume
void setPasswordFile(fstream& file, Volume*& volume, vector<Entry*>& entries) {
	string filename;
	string name = "";
	string extension = "";


	cout << "\nInput file name to set the password(example:text.txt): ";
	cin >> filename;

	if (!checkFileName(filename, name, extension)) {
		cout << "\nFile name is error!!!";
		return;
	} else {
		int file_pos = -1; // the file's position
		int num_entries = SECTORS_OF_ENTRY * BYTES_OF_SECTOR / sizeof(Entry);
		for (int i = 0; i < num_entries; i++) {
			if (entries[i]->nameEntry == name && entries[i]->nameExtensionEntry == extension) {
				file_pos = i;
				break;
			}
		}

		if (file_pos == -1) {
			cout << "\nThe " << filename << " is not exist in this volume!";
			return;
		} else {
			if (entries[file_pos]->passwordEntry.empty()) {
				cout << "\nInpur the password for this file: ";
				cin >> entries[file_pos]->passwordEntry;

				int entry_pos = (SECTORS_BEFORE_FAT + SECTORS_OF_FAT) * BYTES_OF_SECTOR + file_pos * sizeof(Entry);
				file.seekp(entry_pos, ios::beg);
				file.write((char*)entries[file_pos], sizeof(Entry));
				file.flush();
				cout << "\nThe password had been success!";
			} else {
				cout << "\nThe password has set before!";
			}

		}
	}
}

// define the funtion to change the password for the file
void changePasswordFile(fstream& file, Volume*& volume, vector<Entry*>& entries) {
	string filename;
	string name = "";
	string extension = "";


	cout << "\nInput the file which need change the password(example:text.txt): ";
	cin >> filename;

	if (!checkFileName(filename, name, extension)) {
		cout << "\nFile name is error!!!";
		return;
	}

	else {
		int file_pos = -1;
		int num_entries = SECTORS_OF_ENTRY * BYTES_OF_SECTOR / sizeof(Entry);
		for (int i = 0; i < num_entries; i++) {
			if (entries[i]->nameEntry == name && entries[i]->nameExtensionEntry == extension) {
				file_pos = i;
				break;
			}
		}

		if (file_pos == -1) {
			cout << "\nThe " << filename << " is not exist in this volume!";
			return;
		} else {

			if (entries[file_pos]->passwordEntry.empty()) {
				cout << "\nPlease set your password before change it";
				return;
			}
			else {
				string password;
				cout << "\nInput your password: ";
				cin >> password;

				if (password != entries[file_pos]->passwordEntry) {
					cout << "\nYour password is not correct!";
					return;
				}

				cout << "\nInput your new password: ";
				cin >> entries[file_pos]->passwordEntry;

				int entry_pos = (SECTORS_BEFORE_FAT + SECTORS_OF_FAT) * BYTES_OF_SECTOR + file_pos * sizeof(Entry);
				file.seekp(entry_pos, ios::beg);
				file.write((char*)entries[file_pos], sizeof(Entry));
				file.flush();
				cout << "\nSuccess!";
			}

		}
	}
}

