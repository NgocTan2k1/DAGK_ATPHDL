#include "listFiles.h"

// Hàm liệt kê các tập tin có trong volume
void listFiles(fstream& file, Volume*& vol, int*& fat, vector<Entry*>& entries) {
	int num_entries = SECTORS_OF_ENTRY * BYTES_PER_SECTOR / sizeof(Entry);
	bool no_file = true;
	// Duyệt qua bảng thư mục để in ra các tập tin hiện có
	for (int i = 0; i < num_entries; i++) {
		if (entries[i]->name.empty()) continue;

		no_file = false;
		cout << entries[i]->name + "." + entries[i]->extension << endl;
	}

	if (no_file) {
		cout << endl << "Hien khong co tap tin nao trong volume!!!\n\n";
	}
}