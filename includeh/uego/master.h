#ifndef MASTER_H
#define MASTER_H

////////////////////////////////////////////////////////////
// $Id: master.h,v 2.6 1998/03/29 10:39:46 jelasity Exp $
// master.h
// declares class Master;
////////////////////////////////////////////////////////////
// modification history:
//	Jelasity 98 01 24 new save flag
////////////////////////////////////////////////////////////

// --- flags for save --------------------------------------------------

#define SAVE_INI 1UL
#define SHORT_SAVE 2UL
#include <fstream>
#include "MoveAndRotate.h"
#include "Molecule.h"
#include  "WriteMolecule.h"
#include "Plot.h"
// ---------------------------------------------------------------------

class Master {

private:

	static Ini *_ini;		// parameters given by the user

	SpeciesList *head;		// first element is head->next!
	long length;		// length of species list
	long FunctionEvals;	// number of function evaluations
	short level;		// actual strict level

	void ElitistSelection(long = -1);	// if list too long, shortens it and get with better
	void CheckLength( long =-1 ); // if list too long, shortens it
	void Fuse();			// fuses species list using 'level'
	void _NewSpecies(long);
	void NewSpecies();
	void _Mutate(long);
	void Optimize();
	void Mutate();

	// --- to trace optimization process
	long tracenum;	// counts checkpoints
	char *tracename;
	void CheckPoint();

	// --- this is needed for performing more than 1 experiments -------
	void ReInit(Ini*, char*);
	void Clean();
	void NewSearch() {
		Clean();
		ReInit(_ini, tracename);

	}
	;
	void _Go(); // performs the optimization process

	char FailFlag;

public:

	Master(Ini* ini, char* trace) {
		tracenum = 0;
		ReInit(ini, trace);
	}
	;
	~Master() {
		Clean();
	}
	;

	void Go() {
		NewSearch();
		if (!Fail())
			_Go();
	}
	;
	void Save(FILE*, double = 0UL, unsigned long = 0UL); // saves status of search

	static Ini& ini() {
		return *_ini;
	}
	; // safe access to _ini
	static char iniSet() {
		return _ini != NULL;
	}
	;
	static double M(long, Ini* = _ini);
	// max theoretically possible spec list length at given level

	char Fail() {
		return FailFlag;
	}
	;

	void PrintList();

	void SaveListFile(int level, string file, string step);


	void PlotMoleculesList(int level,  string step);

	void getBestSpecies(short level, bool saveFile);
	double getBestSpecies();
	void getSpeciesList(short level, bool saveFile);
};

#endif
