#include "VirtualFileS.h"
#include <iostream>
#include <string>
using namespace std;



void vfs::creatdisc(char name[20]) {

	ofstream archivo("VirtualFile.bin", ios::in | ios::app | ios::binary);

	if (!archivo) {

		cout << "Incorrecto" << endl;

	}
	else {

		//Metadata
		int num = 0;
		double espacioT;
		cout << "Ingrese Total de Directorios/Archivosa crear" << endl;
		cin >> num;
		//cout << name << endl;
		espacioT = (num * 33308) * 4096;
		espacioT = espacioT / 1024 / 1024;
		double espacioTot;
		MetaData inf;
		strcpy_s(inf.nombreD, name);
		espacioTot = espacioT;
		//cout << espacioTot;
		inf.totalEntradas = num;

		bitmap = new char[inf.totalEntradas * 15];
		for (int i = 0; i < inf.totalEntradas * 15; i++) {

			bitmap[i] = '0';

		}


		inf.EspacioTotal = espacioTot;
		inf.EspacioVacio = 0;
		archivo.write(reinterpret_cast<const char *>(&inf), sizeof(MetaData));

		//archivo.seekp(0, ios::beg);
		//inf.EspacioTotal = espacioTot;
		//inf.EspacioVacio = 10;
		///archivo.write(reinterpret_cast<const char *>(&inf), sizeof(MetaData));
		//Metadata
		/////////////////////
		//Mapa de bits
		bitMap mapaB;
		mapaB.bitmap = bitmap;
		archivo.write(reinterpret_cast<const char *>(&mapaB), sizeof(bitMap));
		//Mapa de Bits



		//FileEntry
		FileEntry entrada;
		char root[30] = "root";
		strcpy_s(entrada.fileName, root);
		entrada.FileType = 'd';
		entrada.ocupada = true;
		entrada.primerhijo = 1;
		entrada.padre = -1;
		entrada.hijoderecho = -1;
		entrada.hijoultimo = 1;

		archivo.write(reinterpret_cast<const char *>(&entrada), sizeof(FileEntry));



		for (int i = 0; i < num - 1; i++) {


			FileEntry salida;

			archivo.write(reinterpret_cast<const char *>(&salida), sizeof(FileEntry));

		}
		//fileEntry

		//Bloque de datos

		for (int i = 0; i < num; i++) {

			for (int j = 0; j < 12; j++) {
				DataBlock_Direct DBD;
				archivo.write(reinterpret_cast<const char *>(&DBD), sizeof(DataBlock_Direct));
			}
			DataBlock_I1 DB1;
			archivo.write(reinterpret_cast<const char *>(&DB1), sizeof(DataBlock_I1));
			DataBlock_I2 DB2;
			archivo.write(reinterpret_cast<const char *>(&DB2), sizeof(DataBlock_I2));
			DataBlock_I3 DB3;
			archivo.write(reinterpret_cast<const char *>(&DB3), sizeof(DataBlock_I3));

		}

		//Bloque de datos

		archivo.close();

	}



}
int vfs::diskInfo() {

	ifstream archivo("VirtualFile.bin", ios::in | ios::binary);

	if (!archivo) {

		cout << "End" << endl;

	}
	else {

		archivo.seekg(0, ios::beg);

		MetaData inf;

		archivo.read(reinterpret_cast<char *>(&inf), sizeof(MetaData));

		//cout << inf.nombreD<<endl;
		//cout << inf.EspacioTotal << endl;
		//cout << inf.EspacioVacio << endl;
		return inf.totalEntradas * 15;


		archivo.close();
	}

}
void vfs::diskInfo2(int padre, char name[30]) {

	ifstream archivo("VirtualFile.bin", ios::in | ios::binary);

	if (!archivo) {

		cout << "End" << endl;

	}
	else {

		archivo.seekg(0, ios::beg);

		int Vacio = -1;
		int Padre = -1;

		bool esPrimerHijo = false;
		bool primerom = false;
		MetaData inf;

		archivo.read(reinterpret_cast<char *>(&inf), sizeof(MetaData));
		//cout<< inf.totalEntradas << endl;

		FileEntry as;
		int size = sizeof(MetaData) + sizeof(bitMap);
		archivo.seekg(size);
		archivo.read(reinterpret_cast<char *>(&as), sizeof(FileEntry));
		//cout << as.fileName << endl;

		int totalE = inf.totalEntradas;

		for (int i = 0; i < totalE; i++) {
			//cout << "Holaaa" << endl;
			//cout << i << endl;

			archivo.seekg(size + (sizeof(FileEntry)*i));
			archivo.read(reinterpret_cast<char *>(&as), sizeof(FileEntry));
			//cout << as.fileName << endl;

			if (i == padre && as.FileType == 'd') {



				//cout << "si lo encontro" << Padre << endl;
				if (as.primerhijo == -1) {

					esPrimerHijo = true;

				}

				if (as.primerhijo != -1) {
					esPrimerHijo = false;

				}


			}
			if (as.ocupada == false && Vacio == -1) {

				Vacio = i;

			}




		}




		//cout <<"VACIO:"<< Vacio << endl;
		//cout << "pADRE"<<Padre << endl;

		archivo.close();

		if (Vacio != -1) {
			makedir(Vacio, padre, esPrimerHijo, name);
		}
		else {
			cout << "Esta lleno todo,No hay Espacio para mas entradas" << endl;
		}
	}


}
void vfs::makedir(int Vacio, int Padre, bool esPrimerHijo, char name[30]) {

	fstream archivo("VirtualFile.bin", ios::out | ios::in | ios::binary);

	int size = sizeof(MetaData) + sizeof(bitMap);
	if (!archivo) {
		cout << "erro" << endl;

	}
	else {
		FileEntry as;
		cout << "Seleccione Tipo" << endl;
		cout << "1.d(Directorio)" << endl;
		cout << "2.a(Archivo)" << endl;
		cin >> as.FileType;
		as.ocupada = true;
		//cout << Vacio << endl;
		int c = size + (sizeof(FileEntry)*Vacio);
		//cout << c << endl;
		archivo.seekg(c);
		archivo.read((char*)(&as), sizeof(FileEntry));
		//cout << "h::::" << as.fileName << endl;
		strcpy_s(as.fileName, name);
		as.padre = Padre;
		as.ocupada = true;
		archivo.seekp(-(sizeof(FileEntry) - archivo.tellp()));

		archivo.write(reinterpret_cast<const char *>(&as), sizeof(FileEntry));
		archivo.seekg(c);
		archivo.read((char*)(&as), sizeof(FileEntry));
		//cout << "h::::" << as.fileName << endl;

		if (esPrimerHijo == true) {
			FileEntry pp;
			archivo.seekg(sizeof(MetaData) + sizeof(bitMap) + (sizeof(FileEntry)*Padre));
			archivo.read(reinterpret_cast<char *>(&pp), sizeof(FileEntry));
			cout << "Guardado Teniendo como padre:" << pp.fileName << endl;
			archivo.seekp(-(sizeof(FileEntry) - archivo.tellp()));
			pp.primerhijo = Vacio;
			pp.hijoultimo = Vacio;
			archivo.write(reinterpret_cast<const char *>(&pp), sizeof(FileEntry));

		}
		else {

			FileEntry pp;
			archivo.seekg(sizeof(MetaData) + sizeof(bitMap) + (sizeof(FileEntry)*Padre));
			archivo.read(reinterpret_cast<char *>(&pp), sizeof(FileEntry));
			cout << "Guardado Teniendo como padre:" << pp.fileName << endl;
			archivo.seekp(-(sizeof(FileEntry) - archivo.tellp()));
			int z = pp.hijoultimo;
			pp.hijoultimo = Vacio;
			archivo.write(reinterpret_cast<const char *>(&pp), sizeof(FileEntry));

			/////////////// Cambio de numero a hermano
			archivo.seekg(sizeof(MetaData) + sizeof(bitMap) + (sizeof(FileEntry)*z));
			archivo.read(reinterpret_cast<char *>(&pp), sizeof(FileEntry));
			archivo.seekp(-(sizeof(FileEntry) - archivo.tellp()));
			archivo.write(reinterpret_cast<const char *>(&pp), sizeof(FileEntry));

		}

	}
	archivo.close();


}
int vfs::retN() {

	fstream archivo("VirtualFile.bin", ios::out | ios::in | ios::binary);
	MetaData as;
	archivo.seekg(ios::beg, 0);
	archivo.read(reinterpret_cast<char*>(&as), sizeof(MetaData));
	return as.totalEntradas * 15;

}
void vfs::wrtB(char * bitmap) {
	int size = sizeof(MetaData);

	fstream archivo("VirtualFile.bin", ios::out | ios::in | ios::binary);
	bitMap as;
	archivo.seekg(size);
	archivo.read(reinterpret_cast<char*>(&as), sizeof(bitMap));
	archivo.seekp(-(sizeof(bitMap) - archivo.tellp()));
	as.bitmap = bitmap;
	archivo.write(reinterpret_cast<const char*>(&as), sizeof(MetaData));
	archivo.close();

}
string vfs::sN(int n) {
	FileEntry as;
	fstream archivo("VirtualFile.bin", ios::out | ios::in | ios::binary);
	archivo.seekg(sizeof(MetaData) + sizeof(bitMap) + (sizeof(FileEntry)*n));
	archivo.read(reinterpret_cast<char *>(&as), sizeof(FileEntry));

	return as.fileName;


}

bool vfs::rm(int pos) {

	FileEntry actual;
	fstream archivo("VirtualFile.bin", ios::in | ios::out | ios::binary);
	int size = sizeof(MetaData) + sizeof(bitMap);
	archivo.seekg(size + (sizeof(FileEntry)*pos));
	archivo.read(reinterpret_cast<char *>(&actual), sizeof(FileEntry));
	if (actual.FileType == 'a') {
		archivo.seekg(size + (sizeof(FileEntry)*actual.padre));
		archivo.read(reinterpret_cast<char *>(&actual), sizeof(FileEntry));



		archivo.seekp(-(sizeof(FileEntry) - archivo.tellp()));
	}
	return true;

}

int vfs::cdRegreso(int n) {
	FileEntry regreso;
	ifstream archivo("VirtualFile.bin", ios::in | ios::out | ios::binary);
	int size = sizeof(MetaData) + sizeof(bitMap);
	archivo.seekg(size + (sizeof(FileEntry)*n));
	archivo.read(reinterpret_cast<char *>(&regreso), sizeof(FileEntry));

	if (regreso.padre != -1) {

		return regreso.padre;
	}
	else {

		cout << "Estas en el root no te puedes regresar" << endl;
		return 0;

	}
}

int vfs::cd(char name[30], int padre) {


	ifstream archivo("VirtualFile.bin", ios::in | ios::binary);

	if (!archivo) {

		cout << "Mal" << endl;
		return padre;
	}
	else {
		archivo.seekg(0, ios::beg);


		MetaData inf;

		archivo.read(reinterpret_cast<char *>(&inf), sizeof(MetaData));
		//cout << inf.totalEntradas << endl;
		int totalE = inf.totalEntradas;
		FileEntry as;
		int size = sizeof(MetaData) + sizeof(bitMap);
		archivo.seekg(size);
		archivo.read(reinterpret_cast<char *>(&as), sizeof(FileEntry));
		//cout << as.fileName << endl;



		for (int i = 0; i < totalE; i++) {
			//cout << "Holaaa" << endl;
			//cout << i << endl;

			archivo.seekg(size + (sizeof(FileEntry)*i));
			archivo.read(reinterpret_cast<char *>(&as), sizeof(FileEntry));



			//cout << as.fileName << "|" << name << endl;



			if (strcmp(as.fileName, name) == 0 && as.padre == padre) {

				cout << "Nombre:" << as.fileName << endl;
				return i;
			}



		}

		if (strcmp(as.fileName, name) != 0) {
			cout << "No hay ningun subdirectorio con ese nombre" << endl;
		}


	}
	return padre;
}

void vfs::importar(char name[100], int padre) {


	fstream arL;
	fstream asE("VirtualFile.bin", ios::in | ios::out | ios::binary);
	arL.open(name, ios::in);

	if (!arL | !asE) {
		cout << "error" << endl;


	}
	else {

		int j = 0;

		int Bp1 = 0;
		int Bp2 = 0;
		int Bp3 = 0;
		int ocupados[15];
		asE.seekg(0, ios::beg);

		MetaData inf;
		DataBlock_Direct infor;
		DataBlock_I1 ing;
		DataBlock_I2 ing2;
		asE.read(reinterpret_cast<char *>(&inf), sizeof(MetaData));
		int totalE = inf.totalEntradas;
		int p = 0;
		for (int i = 15 * padre; i < i + 15; i++) {
			if (bitmap[i] = '0') {


				break;
			}
			p++;
		}
		int lim = p + 15;
		int size = inf.totalEntradas;
		int posi = 0;
		for (int i = 0; i < posi; i++) {
			asE.seekg(sizeof(MetaData) + sizeof(bitMap) + (sizeof(FileEntry)*totalE) + ((sizeof(DataBlock_Direct) * 12)
				+ sizeof(DataBlock_I1) + sizeof(DataBlock_I2) + sizeof(DataBlock_I3)*p));

			if (p == 12) {
				asE.seekg(sizeof(MetaData) + sizeof(bitMap) + (sizeof(FileEntry)*totalE) + (sizeof(DataBlock_Direct) * 12));
				asE.read(reinterpret_cast<char *>(&ing), sizeof(DataBlock_I1));
				while (!arL.eof() || j < 4096) {

					arL.get(ing.points[Bp1].data[j]);

					j = j + 1;
					if (j == 4096) {
						Bp1 = Bp1 + 1;
						j = 0;
					}
					if (Bp1 == 15) {
						p = p + 1;
						Bp1 = 0;
					}
				}
				if (p == 13) {
					asE.seekg(sizeof(MetaData) + sizeof(bitMap) + (sizeof(FileEntry)*totalE) + (sizeof(DataBlock_Direct) * 12) + sizeof(DataBlock_I1));
					asE.read(reinterpret_cast<char *>(&ing2), sizeof(DataBlock_I2));

					while (!arL.eof() || j < 4096) {

						arL.get(ing2.points[Bp2].data[j]);
						bitmap[(15 * padre) + 12] = '1';
						j = j + 1;
						if (j == 4096) {
							Bp2 = Bp2 + 1;
							j = 0;
						}
						if (Bp2 == 32) {
							p = p + 1;

						}

					}
					if (p == 14) {
						asE.seekg(sizeof(MetaData) + sizeof(bitMap) + (sizeof(FileEntry)*totalE) + (sizeof(DataBlock_Direct) * 12) + sizeof(DataBlock_I1));
						asE.read(reinterpret_cast<char *>(&ing2), sizeof(DataBlock_I2));

						while (!arL.eof() || j < 4096) {

							arL.get(ing2.points[Bp3].data[j]);
							bitmap[(15 * padre) + 12] = '1';
							j = j + 1;
							if (j == 4096) {
								Bp3 = Bp3 + 1;
								j = 0;
							}
							if (Bp2 == 32) {
								p = 0;
								cout << "se ocuparon todos los espacios" << endl;
							}

						}
					}



				}




			}
		}
	}
}

void vfs::ls(int padre) {

				ifstream archivo("VirtualFile.bin", ios::in | ios::binary);
				MetaData inf1;
				archivo.read(reinterpret_cast<char *>(&inf1), sizeof(MetaData));
				//cout << inf1.totalEntradas << endl;
				int totalE = inf1.totalEntradas;
				FileEntry inf;
				archivo.seekg(sizeof(MetaData) + sizeof(bitMap) + (sizeof(FileEntry)* padre));
				archivo.read(reinterpret_cast<char *>(&inf), sizeof(FileEntry));
				//cout << inf.primerhijo << endl;
				//cout << padre << endl;
				if (!archivo) {

					cout << "error" << endl;

				}
				else {
					FileEntry as;
					int size = sizeof(MetaData) + sizeof(bitMap);
					archivo.seekg(size);
					archivo.read(reinterpret_cast<char *>(&as), sizeof(FileEntry));
					//cout << as.fileName << endl;


					bool coincide = true;

					for (int i = 0; i < totalE; i++) {

						archivo.seekg(size + (sizeof(FileEntry)*i));
						archivo.read(reinterpret_cast<char *>(&as), sizeof(FileEntry));

						if (as.padre == padre) {

							cout << "Nombre:" << as.fileName << endl;
							coincide = false;
						}

					}
					if (coincide == true) {

						cout << "No se encontraron registros dentro del directorio" << endl;

					}

				}
			}
	
	