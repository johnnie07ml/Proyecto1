#include <conio.h>
#include "VirtualFileS.h"

int main() {

	vfs file;
	int pos = 0;;
	char * bitMap;
	cout << "Bienvenido a la Terminal" << endl;
	bool ing = true;
	cout << "Nombre para CrearDisco" << endl;
	cout << "CreateDisk:";
	char nombreDisco[20];
	cin >> nombreDisco;
	file.creatdisc(nombreDisco);

	bitMap = new char[file.diskInfo()];

	for (int i = 0; i < file.diskInfo(); i++) {

		bitMap[i] = '0';

	}
	//cout << file.diskInfo()<<endl;
	file.wrtB(bitMap);
	do {
		char comand[30];
		cout << "Cd:" << file.sN(pos) << endl;
		cout << "Command:";
		cin >> comand;
		//cout << pos;



		if (strcmp(comand, "mkdir") == 0) {

			cout << "mkdir:";
			char nombreR[30];
			cin >> nombreR;





			file.diskInfo2(pos, nombreR);

		}

		if (strcmp(comand, "ls") == 0) {

			file.ls(pos);

		}
		if (strcmp(comand, "cd") == 0) {

			cout << "cd:";
			char nomcd[30];
			cin >> nomcd;
			if (strcmp(nomcd, "..") == 0) {
				pos = file.cdRegreso(pos);
			}
			else {
				pos = file.cd(nomcd, pos);
			}

		}



		if (strcmp(comand, "salir") == 0) {
			ing = false;
		}



	} while (ing == true);
	_getch();

	return 0;
}
