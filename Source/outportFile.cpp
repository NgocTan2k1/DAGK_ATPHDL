#include "outportFile.h"


// Hàm tách tên và phần mở rộng file 
pair<string, string> readFile(string filename) {
	string name, extension;

	int pos = filename.find('.');
	for (int i = 0; i < pos; i++) {
		name.push_back(filename[i]);
	}

	for (int i = pos + 1; i < filename.length(); i++) {
		extension.push_back(filename[i]);
	}

	return { name, extension };
}


// Hàm outport file từu volume ra bên ngoài
void outportFile(fstream& file, Volume*& vol, int*& fat, vector<Entry*>& entries) {
	string filename;

	while (true) {
		system("cls");
		cout << "Nhap ten file trong volume can chep ra ngoai (Ex: test.txt): ";
		cin >> filename;

		if (filename.find(".") == string::npos) {
			cout << "Ten file khong hop le.\nXin vui long nhap lai\n";
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
		cout << "Oops!!! Khong ton tai tap tin " << filename << " trong volume\n\n";
		return;
	}

	ofstream outfile;
	outfile.open(filename, ios::binary);

	int cluster_pos = entries[entry_pos_file]->cluster_begin;
	int size_file = entries[entry_pos_file]->size;
	while (true) {
		int system_bytes = (SECTORS_BEFORE_FAT + SECTORS_OF_FAT + SECTORS_OF_ENTRY) * BYTES_PER_SECTOR;
		
		for (int i = 0; i < SECTORS_PER_CLUSTER; i++) {
			char* sector = new char[BYTES_PER_SECTOR];
			int read_pos = system_bytes + (cluster_pos * SECTORS_PER_CLUSTER + i) * BYTES_PER_SECTOR;
			//file.seekg(read_pos, ios::beg);
			//file.read(sector, BYTES_PER_SECTOR);
			sector = readSector(file, read_pos);
			// Size lớn hơn số byte của 1 sector thì ghi dữ liệu 1 sector 
			if (size_file > BYTES_PER_SECTOR) {
				outfile.write(sector, BYTES_PER_SECTOR);
				size_file -= BYTES_PER_SECTOR;
			}
			// Size nhỏ hơn số byte 1 sector thì ghi từng byte xong thoát khỏi vòng lặp và kết thúc
			else {
				outfile.write(sector, size_file);
				size_file = 0;
				break;
			}
		}


		if (size_file == 0) {
			break;
		}


		// Chưa ghi xong thì tìm tới cluster tiếp theo dạng danh sách liên kết để tiếp tục ghi
		cluster_pos = fat[cluster_pos];
	}

	cout << "\nTap tin " << filename << " da xuat thanh cong!\n\n";
}

