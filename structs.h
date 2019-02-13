#pragma once
#ifndef structs_h
#define structs_h

#include <fstream>
#include <iostream>
#include <string>
using namespace std;


struct Date {
	int day = 2;
	int mon = 9;
	int year2019;
};
struct FileEntry {

	char fileName[30] = "hh";
	char FileType; //d: directorio f:file
	Date FechaCreacion;
	int padre;
	int primerhijo = -1;
	int hijoderecho = -1;
	int hijoultimo = -1;
	bool ocupada = false;
	unsigned int Size;
	unsigned int DB_Directs[12];
	unsigned int DB_Indirects[3];
};

struct DataBlock_Direct {
	char data[4096];
};
struct DataBlock_I1 {

	DataBlock_Direct points[16];
};
struct DataBlock_I2 {
	DataBlock_Direct points[32];
};
struct DataBlock_I3 {
	DataBlock_Direct points[64];
};
struct MetaData {

	char nombreD[20];
	double EspacioTotal, EspacioVacio;//Metadata
	int totalEntradas;
	//Date FechaCreacion;
};

struct bitMap {
	int CantidadDeBloques;
	int BlockeSize;
	int discsSize;
	char * bitmap;
};
#endif // !1