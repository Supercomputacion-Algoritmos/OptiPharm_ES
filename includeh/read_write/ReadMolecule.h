
/* 
 * File:   readMolecule.h
 * Author: savins
 *
 * Created on 2 de enero de 2016, 10:50
 */

#ifndef READMOLECULE_H
#define READMOLECULE_H

#include <vector>
#include "Molecule.h"
#include "Atom.h"
#include "Bond.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <stdlib.h>     /* exit, EXIT_FAILURE */
#include <ctype.h>
#include "uego.h"
using namespace std;


class ReadMolecule {
public:


    void static readMol(char* filePath , Molecule *molecule, bool considerHydrogens, bool sameVanDerWaalsRadius);

    void static readDBMol2(char* filePath , vector<Molecule> *conformations, bool considerHydrogens, bool sameVanDerWaalsRadius);


};

#endif /* READMOLECULE_H */

