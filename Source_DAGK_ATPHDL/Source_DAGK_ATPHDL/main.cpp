#include "Volume.h"
#include "FilesList.h"
#include "Password.h"
//#include "importFile.h"
//#include "outportFile.h"
//#include "deleteFile.h"


void Menu() {
	cout << "\t=========== Menu ==========";
	cout << "\n1. Create / Format the MyFS.DRS file";
	cout << "\n2. Set / Check / Change the password of the MyFS.DRS file";
	cout << "\n3. Show all file in the MyFS.DRS file";
	cout << "\n4. Set / Change a file's password in the MyFS.DRS file";
	cout << "\n5. Import a file from outside the MyFS.DRS file";
	cout << "\n6. Outport a file from the MyFS.DRS file to outside";
	cout << "\n7. Delete a file in the MyFS.DRS";
	cout << "\n0. Exit program!";
	cout << "\n\t============ End ===========";
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

		cout << "\nInput your selection: ";
		cin >> option;
		
		while (option < 0 || option > 7) {
			system("cls");
			Menu();
			cout << "\n\n================ ERROR ================";
			cout << "\nPlease input your selection again: ";
			cin >> option;
		}
		if (option == 0) {
			system("cls");
			cout << "====== Notify ======";
			cout << "\nExit the program!\n";
			break;
			system("pause");
		}
		/*else if (option == 1) {
			formatVolumeDRS("MyFS", "DRS", vol, fat, entries);
			cout << "\nTao volume thanh cong!\n\n";
			system("pause");
		}*/
		/*else if (option == 2) {
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
		}*/


	}


	return 0;
}
