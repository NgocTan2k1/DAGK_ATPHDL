#include "formatVolume.h"
#include "listFiles.h"
#include "password.h"
#include "importFile.h"
#include "outportFile.h"
#include "deleteFile.h"


void Menu() {
	cout << "1. Tao/Dinh dang volume MyFS.DRS\n\n";
	cout << "2. Thiet lap/Kiem tra/Doi mat khau truy xuat MyFS\n\n";
	cout << "3. Liet ke danh sach cac tap tin trong MyFS\n\n";
	cout << "4. Dat/Doi mat khau truy xuat cho 1 tap tin trong MyFS\n\n";
	cout << "5. Chep(Import) 1 tap tin tu ben ngoai vao MyFS\n\n";
	cout << "6. Chep(Outport) 1 tap tin trong MyFS ra ngoai\n\n";
	cout << "7. Xoa 1 tap tin trong MyFS\n\n";
	cout << "0. Thoat\n\n";
}

int main() {
	int option;
	fstream file;
	Volume* vol = NULL;
	int* fat = NULL;
	vector<Entry*> entries;

	while (true) {
		system("cls");
		Menu();

		cout << "\nNhap lua chon chuc nang: ";
		cin >> option;

		if (option < 0 || option > 7) {
			cout << "\nLua chon khong hop le. Xin vui long nhap lai!\n\n";
			system("pause");
		}
		else if (option == 0) {
			cout << "\nThoat thanh cong!\n\n";
			break;
			system("pause");
		}
		else if (option == 1) {
			formatVolume("MyFS", "DRS", vol, fat, entries);
			cout << "\nTao volume thanh cong!\n\n";
			system("pause");
		}
		else if (option == 2) {
				int choice;
				do {
					system("cls");
					cout << "\n1. Thiet lap mat khau\n\n";
					cout << "2. Kiem tra mat khau\n\n";
					cout << "3. Doi mat khau\n\n";
					cout << "0. Thoat";

					cout << "\n\n--> Nhap lua chon: ";
					cin >> choice;

					if (choice == 1) {
						setPasswordVolume(file, vol);
					}
						else if (choice == 2) {
						if (checkPasswordVolume(vol)) {
							cout << "\nMat khau chinh xac!\n\n";

						}
						else {
							cout << "\nMat khau khong chinh xac!\n\n";
						}


					}
					else if (choice == 3) {
						changePasswordVolume(file, vol);
					}


					system("pause");

				} while (choice != 0);
			}

		else if (option == 3) {
				cout << "\nDanh sach tap tin hien co trong volume:\n";
				listFiles(file, vol, fat, entries);
				system("pause");
			}

		else if (option == 4) {
				int choice;
				do {
					system("cls");
					cout << "\n1. Thiet lap mat khau tap tin\n\n";
					cout << "2. Doi mat khau tap tin\n\n";
					cout << "0. Thoat";
					cout << "\n\n--> Nhap lua chon: ";
					cin >> choice;

					if (choice == 1) {
						setPasswordFile(file, vol, entries);
					}

					else if (choice == 2) {
						changePasswordFile(file, vol, entries);
					}


					system("pause");

				} while (choice != 0);
			}

		else if (option == 5) {
			file.open("MyFS.DRS", ios::binary | ios::in | ios::out);


			if (!file.is_open()) {
				cout << "\nVolume MyFS.DRS bi loi!\n\n";
				system("pause");
				continue;
			}
				importFile(file, vol, fat, entries);
				system("pause");
				file.close();
		}

		else if (option == 6) {
			file.open("MyFS.DRS", ios::binary | ios::in | ios::out);
			
			if (!file.is_open()) {
				cout << "\nVolume MyFS.DRS bi loi!\n\n";
				system("pause");
				continue;
			}
				outportFile(file, vol, fat, entries);
				system("pause");
				file.close();
		}

		else {
				deleteFile(file, vol, fat, entries);
				cout << "\nDanh sach tap tin hien co trong volume:\n";
				listFiles(file, vol, fat, entries);
				system("pause");
		}
	
		
	}


	return 0;
}
