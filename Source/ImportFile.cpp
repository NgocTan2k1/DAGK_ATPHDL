#include "importfile.h"


// Kiểm tra và lấy tên & phần mở rộng file 
pair<string, string> readFilePath(string file_path) {
	string name;
	string extension;
	int length_path = file_path.length();

	int point_pos = file_path.find('.');
	for (int i = point_pos + 1; i < length_path; i++) {
		extension.push_back(file_path[i]);
	}

	for (int i = point_pos - 1; i >= 0; i--) {
		if (file_path[i] == '/' || file_path[i] == '\\') {
			for (int j = i + 1; j < point_pos; j++) {
				name.push_back(file_path[j]);
			}

			break;
		}
	}

	return { name, extension };
}


// Hàm import file từ bên ngoài vào trong volume
void importFile(fstream& file, Volume*& vol, int*& fat, vector<Entry*>& entries) {
	string filepath;
	fstream fileImport;

	

	while (true) {
		system("cls");
		cout << "Nhap duong dan cua file can chep vao volume(Ex: E:\\Test.txt): ";
		cin >> filepath;

		if (filepath.find(".") == string::npos) {
			cout << "Duong dan file khong hop le.\nXin vui long nhap lai\n\n";
			system("pause");

		}
		else {
			break;
		}
	}


	fileImport.open(filepath, ios::binary | ios::in);
	if (!fileImport.is_open()) {
		cout << "Khong ton tai duong dan file " << filepath << "\n\n";
		return;
	}

	Entry* entry_file = createEntry("", "");
	pair<string, string > res = readFilePath(filepath);
	entry_file->name = res.first; 
	entry_file->extension = res.second; 
	
	
	// Đọc thông tin của volume MyFS.DRS
	file.seekg(0, ios::beg);
	file.read((char*)vol, sizeof(Volume));
	file.seekg(SECTORS_BEFORE_FAT * BYTES_PER_SECTOR, ios::beg);
	file.read((char*)fat, CLUSTERS_OF_VOLUME);
	
	
	int num_entries = SECTORS_OF_ENTRY * BYTES_PER_SECTOR / sizeof(Entry);
	
	file.seekg((SECTORS_BEFORE_FAT + SECTORS_OF_FAT)* BYTES_PER_SECTOR, ios::beg);
	for (int i = 0; i < num_entries; i++) {
		file.read((char*)entries[i], sizeof(Entry));
	}
	


	// Kiểm tra còn entry trống trong volume hay không
	int entry_pos = -1;
	for (int i = 0; i < num_entries; i++) {
		if (entries[i]->name.empty()) {
			entry_pos = i;
			break;
		}
	}

	if (entry_pos == -1) {
		cout << "\nKhong con entry trong volume de ghi file!!!\n\n";
		return;
	}

	// Kiểm tra còn đủ cluster trống trong bảng fat hay không
	int cluster_pos = -1;
	for (int i = 0; i < CLUSTERS_OF_VOLUME; i++) {
		if (fat[i] == CLUSTER_EMPTY) {
			cluster_pos = i;
			break;
		}
	}

	if (cluster_pos == -1) {
		cout << "\nKhong con du cluster de ghi file!!!\n\n";
		return;
	}


	// Ghi thông tin kích thước tập tin
	fileImport.seekg(0, ios::end);
	entry_file->size = fileImport.tellg();

	// Cập nhật con trỏ file tại vị trí bắt đầu tập tin
	fileImport.seekg(0, ios::beg);

	// Ghi vị trí đầu tiên của file trong vùng Data vào cluster trống trong volume
	entry_file->cluster_begin = cluster_pos;

	while (true) {

		// Tính số byte hệ thống
		int system_bytes = (SECTORS_BEFORE_FAT+SECTORS_OF_FAT+SECTORS_OF_ENTRY)*BYTES_PER_SECTOR;
		
		
		// Đọc từng sector sau đó ghi vào file
		for (int i = 0; i < SECTORS_PER_CLUSTER; i++) {
			char* sector = new char[BYTES_PER_SECTOR];
			fileImport.read(sector, BYTES_PER_SECTOR);

			int write_pos = system_bytes + (cluster_pos * SECTORS_PER_CLUSTER + i) * BYTES_PER_SECTOR;
			
			//.seekp(write_pos, ios::beg);
			//file.write(sector, BYTES_PER_SECTOR);
			writeSector(file, sector, write_pos);
		}


		// Kiểm tra nếu tập tin đã ghi hết thì cập nhật bảng fat xong thoát khỏi vòng lặp
		if (fileImport.eof()) {
			fat[cluster_pos] = CLUSTER_NOT_EMPTY;
			break;
		}

		// Nếu chưa đọc hết tập tin thì tiếp tục tìm cluster trống tiếp theo bằng việc tăng vị trí cluster hiện tại lên 1 đơn vị 
		int curr_cluster_pos = cluster_pos + 1;
		while (curr_cluster_pos < CLUSTERS_OF_VOLUME) {
			if (fat[curr_cluster_pos] == CLUSTER_EMPTY) {
				// Cập nhật giá trị bảng fat tại vị trí cluster_pos
				fat[cluster_pos] = curr_cluster_pos;

				// Cập nhật vị trí cluster trống tiếp theo để tiếp tục việc ghi nội dung file
				cluster_pos = curr_cluster_pos;

				break;
			}

			curr_cluster_pos++;
		}

		// Nếu không còn cluster trống thì in ra thông tin đầy bộ nhớ lên console
		if (curr_cluster_pos == CLUSTERS_OF_VOLUME) {
			cout << "\nVolume MyFS da day bo nho!!!\n\n";
			return;
		}
	}


	// Ghi thông tin file vào entry trống trong volume 
	entries[entry_pos] = entry_file;
	int empty_entry_pos = (SECTORS_BEFORE_FAT + SECTORS_OF_FAT ) * BYTES_PER_SECTOR + entry_pos * sizeof(Entry);
	file.seekp(empty_entry_pos, ios::beg);
	file.write((char*)entries[entry_pos], sizeof(Entry));

	// Cập nhật bảng fat vào file
	file.seekp(SECTORS_BEFORE_FAT * BYTES_PER_SECTOR, ios::beg);
	file.write((char*)fat, CLUSTERS_OF_VOLUME);
	



	cout << "\nImport tap tin vao volume thanh cong!\n\n";

}
