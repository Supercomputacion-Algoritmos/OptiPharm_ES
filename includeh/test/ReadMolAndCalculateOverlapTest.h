

/*
 * File:   readMolAndCalculateOverlap.h
 * Author: savins
 *
 * Created on 24 de febrero de 2016, 12:36
 */

#ifndef READMOLANDCALCULATEOVERLAP_H
#define READMOLANDCALCULATEOVERLAP_H
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <math.h>

#include "Molecule.h"
#include "Atom.h"
#include "Bond.h"
#include "ReadMolecule.h"
#include "VolumeOverlap.h"
#include "ReadMoleculeSD.h"

class ReadMolAndCalculateOverlapTest {
public:
	void static mol2AllOverlapOneMolecule(string file);
	void static mol2AllOverlapTwoMolecule(string file1, string file2);
	void static sdHSAnalyticTwoMol(string file);

	int static testMol2AllOverlapOneMolecule(void);
	int static testMol2AllOverlapTwoMolecule(void);
	int static testSDHSAnalyticTwoMolecule(void);
private:
};
#endif /* READMOLANDCALCULATEOVERLAP_H */

