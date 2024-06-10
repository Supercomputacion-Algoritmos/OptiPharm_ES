/*
 * Parameters.h
 *
 *  Created on: 16 jan. 2017
 *      Author: Savins
 */

#ifndef SRC_FUNCTIONS_PARAMETERS_H_
#define SRC_FUNCTIONS_PARAMETERS_H_

#include "uego.h"
#include "usrintf.h"
#include <string>
#include <string.h>
#include "Point3DDouble.h"
#include "VolumeOverlap.h"
#include "Molecule.h"
#include <cmath> // Para abs(double)

// Align molecules according the three axis.
#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/SVD>
using namespace Eigen;

class Parameters {
public:
	struct Configuration {
		bool updateConfig = false;
		bool onlyConfig = true;
		bool doPrePCA = false;
		bool pointOneZero = false;
		bool saveMolFile = false;
		bool considerHidrogens = false;
		bool sameRadiusVdW = true;
	};
	int static readArguments(int argc, char** argv, int* indexArray, int lenghtIndexArray, Configuration* c);
	string static getNameMol(char* path);
	void static defineDeltaMovement(Ini* ini, double *minX, double *maxX, double *minY, double *maxY, double *minZ, double *maxZ);
	void static alignMolecule(Molecule * mol);

};

#endif /* SRC_FUNCTIONS_TANIMOTO_H_ */
