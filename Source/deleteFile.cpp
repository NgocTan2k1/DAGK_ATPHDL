#include "deleteFile.h"


// Hàm xóa file trong volume
void deleteFile(fstream& file, Volume*& vol, int*& fat, vector<Entry*>& entries) {
	string filename;

	while (true) {
		system("cls");
		cout << "\nNhap ten file trong volume can xoa (Ex: test.txt): ";
		cin >> filename;

		if (filename.find(".") == string::npos) {
			cout << "\nTen file khong hop le. Xin vui long nhap lai!\n\n";
			system("pause");

		}
		else {
			break;
		}
	}


	// Đọc thông tin của volume MyFS.DRS
	file.seekg(0, ios::beg);
	file.read((char*)vol, sizeof(Volume));
	file.seekg(SECTORS_BEFORE_FAT * BYTES_PER_SECTOR, ios::beg);
	file.read((char*)fat, CLUSTERS_OF_VOLUME);

	int num_entries = SECTORS_OF_ENTRY * BYTES_PER_SECTOR / sizeof(Entry);
	file.seekg((SECTORS_BEFORE_FAT + SECTORS_OF_FAT) * BYTES_PER_SECTOR, ios::beg);
	for (int i = 0; i < num_entries; i++) {
		file.read((char*)entries[i], sizeof(Entry));
	}
	

	// Dựa vào tên file và đuôi file để xác định vị trí file trong bảng RDET
	pair<string, string> res = readFile(filename);
	int entry_pos_file = -1;

	for (int i = 0; i < num_entries; i++) {
		if (entries[i]->name == res.first && entries[i]->extension == res.second) {
			entry_pos_file = i;
			break;
		}
	}

	// Nếu không có file này trong RDET thì in ra lỗi
	if (entry_pos_file == -1) {
		cout << "\nTap tin " << filename << " khong ton tai trong volume\n\n";
		return;
	}


	int cluster = entries[entry_pos_file]->cluster_begin;
	while (cluster != CLUSTER_EMPTY) { 
		if (fat[cluster] == CLUSTER_NOT_EMPTY)
		{
			fat[cluster] = CLUSTER_EMPTY;
			break;
		}
		int next_cluster = fat[cluster];
		fat[cluster] = CLUSTER_EMPTY;
		cluster = next_cluster;
	}
	
	
	// Viết entry trống vào file
	int entry_pos = (SECTORS_BEFORE_FAT + SECTORS_OF_FAT)*BYTES_PER_SECTOR + entry_pos_file * sizeof(Entry);
	Entry* del_entry = createEntry("", "");
	entries[entry_pos_file] = del_entry;
	file.seekp(entry_pos, ios::beg);
	file.write((char*)del_entry, sizeof(Entry));

	// Cập nhật lại bảng fat
	file.seekp(SECTORS_BEFORE_FAT*BYTES_PER_SECTOR, ios::beg);
	file.write((char*)fat, CLUSTERS_OF_VOLUME);

	cout << "\nXoa tap tin " << filename << " thanh cong!\n\n";
}
