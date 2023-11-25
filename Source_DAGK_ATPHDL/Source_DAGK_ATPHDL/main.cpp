#include "Volume.h"
#include "Files.h"
//#include "importFile.h"
//#include "outportFile.h"
//#include "deleteFile.h"


void Menu() {
	cout << "\t========================= MAIN MENU =========================";
	cout << "\n1. Create / Format the MyFS.DRS file";
	cout << "\n2. Set / Check / Change the password of the MyFS.DRS file";
	cout << "\n3. Show all file in the MyFS.DRS file";
	cout << "\n4. Set / Change for a file's password in the MyFS.DRS file";
	cout << "\n5. Import a file from outside the MyFS.DRS file";
	cout << "\n6. Outport a file from the MyFS.DRS file to outside";
	cout << "\n7. Delete a file in the MyFS.DRS";
	cout << "\n0. Exit program!";
	cout << "\n\t============================ END ============================";
}

void SubMenu1() {
	cout << "\t=========== SET / CHECK / CHANGE the password of the MyFS.DRS file ==========";
	cout << "\n1. Set password";
	cout << "\n2. Check password";
	cout << "\n3. Change password";
	cout << "\n0. Exit!";
	cout << "\n\t===================================== END ===================================";
}

void SubMenu2() {
	cout << "\t=========== SET / CHANGE a file's password in the MyFS.DRS file ==========";
	cout << "\n1. Set password";
	cout << "\n2. Change password";
	cout << "\n0. Exit!";
	cout << "\n\t================================== END ==================================";
}

int main() {
	int option;
	int selection;
	long sizeVolume;
	fstream file;
	Volume* vol = NULL;
	int* fat = NULL;
	vector<Entry*> entries;

	while (true) {
		system("cls");
		Menu();

		cout << "\nInput your option: ";
		cin >> option;
		
		while (option < 0 || option > 7) {
			system("cls");
			Menu();
			cout << "\n\t=========================== ERROR ===========================";
			cout << "\nPlease input your option again: ";
			cin >> option;
		}

		if (option == 0) {
			system("cls");
			cout << "====== NOTICE ======";
			cout << "\nExit the program!\n";
			break;
			system("pause");
		}
		
		if (option == 1) {
			system("cls");
			cout << "\n\t\t========== CREATE / FORMAT the MyFS.DRS file ==========\n";
			cout << "\nInput the size(clusters) of this volume(<1000): ";
			cin >> sizeVolume;
			while(sizeVolume <= 100 || sizeVolume > 1000) {
				system("cls");
				cout << "\n\t\t========== CREATE / FORMAT the MyFS.DRS file ==========\n";
				if (sizeVolume < 0) {
					cout << "\nThe volume size: " << sizeVolume << " is incorrect!!!";
				}
				if (sizeVolume >= 0 && sizeVolume <= 100) {
					cout << "\nThe volume size: " << sizeVolume << " is so small!!!";
				}
				cout << "\n>NOTE: the volume size should greater than 500 clusters and less than 1000 clusters";
				cout << "\nInput the size(clusters) of this volume: ";
				cin >> sizeVolume;
			}
			formatVolumeDRS("MyFS", "DRS", vol, sizeVolume, fat, entries);
			cout << "\n\t\t===================== Sucessfull! =====================\n\n";
			system("pause");
		}

		if (option == 2) {
			while (true) {
				if (vol == NULL) {
					system("cls");
					cout << "\n\t\t======================= ERROR ========================\n";
					cout << "\n\t\t================== RETURN MAIN MENU ==================\n";
					cout << "\n\t\t======= PLEASE CREATE AND FORMAT VOLUME BEFORE =======\n\n";
					system("pause");
					break;
				}
				system("cls");
				SubMenu1();
				cout << "\nInput your selection: ";
				cin >> selection;

				while (selection < 0 || selection > 3) {
					system("cls");
					SubMenu1();
					cout << "\n\t==================================== ERROR ==================================";
					cout << "\nPlease input your selection again: ";
					cin >> selection;
				}

				if (selection == 1) {
					system("cls");
					cout << "\t\t================ SET PASSWORD FOR VOLUME ================";
					setPasswordVolume(file, vol);
					system("pause");
				}

				if (selection == 2) {
					system("cls");
					cout << "\n\t\t================ CHECK PASSWORD FOR VOLUME ================";

					int check = checkPasswordVolume(vol);

					if (check == 0) {
						cout << "\n\t\t============== The Volume don't have password =============\n\n";
					} 
					if (check == 1) {
						cout << "\n\t\t========================= Correct =========================\n\n";
					}
					
					if (check == 2) {
						cout << "\n\t\t======================== InCorrect ========================\n\n";
					}
					system("pause");
				}

				if (selection == 3) {
					changePasswordVolume(file, vol);
					system("pause");
				}

				if (selection == 0) {
					system("cls");
					cout << "\t\t========== RETURN MAIN MENU ==========\n\n";
					system("pause");
					break;
				}
			}
		}

		if (option == 3) {
			if (vol == NULL) {
				system("cls");
				cout << "\n\t\t======================= ERROR ========================\n";
				cout << "\n\t\t================== RETURN MAIN MENU ==================\n";
				cout << "\n\t\t======= PLEASE CREATE AND FORMAT VOLUME BEFORE =======\n\n";
				system("pause");
			}
			else {
				cout << "\n> All file in this Volume:";
				listFiles(file, vol, fat, entries);
				system("pause");
			}
		}

		if (option == 4) {
			 while (true) {
				 if (vol == NULL) {
					 system("cls");
					 cout << "\t\t======================== ERROR =======================";
					 cout << "\n\t\t================== RETURN MAIN MENU ==================";
					 cout << "\n\t\t======= PLEASE CREATE AND FORMAT VOLUME BEFORE =======\n\n";
					 system("pause");
					 break;
				 }
				 system("cls");
				 SubMenu2();
				 cout << "\nInput your selection: ";
				 cin >> selection;

				 while (selection < 0 || selection > 2) {
					 system("cls");
					 SubMenu2();

					 cout << "\n\t================================= ERROR =================================";
					 cout << "\nPlease input your selection again: ";
					 cin >> selection;
				 }

				 if (selection == 1) {
					 setPasswordFile(file, vol, entries);
					 system("pause");
				 }

				 if (selection == 2) {
					 changePasswordFile(file, vol, entries);
					 system("pause");
				 }

				 if (selection == 0) {
					 system("cls");
					 cout << "\t\t========== RETURN MAIN MENU ==========\n\n";
					 system("pause");
					 break;
				 }
			}
			
		 }	
		
		if (option == 5) {

			if (vol == NULL) {
				system("cls");
				cout << "\n\t\t======================= ERROR ========================\n";
				cout << "\n\t\t================== RETURN MAIN MENU ==================\n";
				cout << "\n\t\t======= PLEASE CREATE AND FORMAT VOLUME BEFORE =======\n\n";
				system("pause");
			} else {
				file.open("MyFS.DRS", ios::binary | ios::in | ios::out);

				if (!file.is_open()) {
					cout << "\nVolume MyFS.DRS is error!\n\n";
					system("pause");
					continue;
				} else {
					importFile(file, vol, sizeVolume, fat, entries);
					system("pause");
				}

				file.close();
			}
		}
		
		else if (option == 6) {
			if(vol == NULL) {
				system("cls");
				cout << "\n\t\t======================= ERROR ========================\n";
				cout << "\n\t\t================== RETURN MAIN MENU ==================\n";
				cout << "\n\t\t======= PLEASE CREATE AND FORMAT VOLUME BEFORE =======\n\n";
				system("pause");
			} else {
				file.open("MyFS.DRS", ios::binary | ios::in | ios::out);

				if (!file.is_open()) {
					cout << "\nThe Volume MyFS.DRS is error!\n\n";
					system("pause");
					continue;
				}

				outportFile(file, vol, sizeVolume, fat, entries);
				system("pause");
				file.close();	
			}
			
			
		}
		if (option == 7) {
			if (vol == NULL) {
				system("cls");
				cout << "\n\t\t======================= ERROR ========================\n";
				cout << "\n\t\t================== RETURN MAIN MENU ==================\n";
				cout << "\n\t\t======= PLEASE CREATE AND FORMAT VOLUME BEFORE =======\n\n";
				system("pause");
			}
			else {
				deleteFile(file, vol, sizeVolume, fat, entries);
				cout << "All current files in this volume:\n";
				listFiles(file, vol, fat, entries);
				system("pause");
			}
			
		}
	}


	return 0;
}
