/*
 * ElectrostaticSimilarity.h
 *
 *  Created on: 16 de may. de 2016
 *      Author: savins
 */

#ifndef SRC_FUNCTIONS_ELECTROSTATICSIMILARITY_H_
#define SRC_FUNCTIONS_ELECTROSTATICSIMILARITY_H_

#include "Molecule.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

class ElectrostaticSimilarity {
public:
	static double callElectrostaticSimilarityPython(Molecule* fixedMol, Molecule* variableMol);
};


#endif /* SRC_FUNCTIONS_ELECTROSTATICSIMILARITY_H_ */
