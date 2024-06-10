/* 
 * File:   VolumeOverlap.h
 * Author: savins
 *
 * Created on 2 de enero de 2016, 10:45
 */

#ifndef VOLUMEOVERLAP_H
#define VOLUMEOVERLAP_H

#include "Molecule.h"
#include "Point3DDouble.h"
#include "Atom.h"
#include "Box.h"
#include <vector>

class VolumeOverlap {
public:

	double static calculateVolumeOverlapHardSphere(Molecule* fixedMol,
			Molecule* variableMol);
	
	double static calculateOverlapVolumeGaussAnalytic(Molecule* fixed,
			Molecule* variable);
	double static calculateOverlapVolumeGaussAnalyticWeight(Molecule* fixed,
			Molecule* variable);

	double static calculateOverlapVolumeGaussAnalyticWEGA(Molecule* fixed,
			Molecule* variable);
	double static calculateOverlapVolumeGaussAnalyticWeightWEGA(Molecule* fixed,
			Molecule* variable);

	Box static getHostGrid(Molecule& mol, double res, double margin);

	double static getOverlapVolumeGaussianNonAnalytic(Molecule& ref,
			Molecule& fit, Box& grid);

	double static tanimoto(Molecule& fixed, Molecule& variable, Box& box);

	double static calculateOverlapVolumeAtomsGaussAnalytic(Atom* atom1,
			Atom* atom2);
	double static calculateWeight(Atom* atom, vector<Atom>* atoms, int iAtom);

	double static calculateOverlapVolumeAtomsWEGA(Atom* atom1,
				Atom* atom2, bool sameVanDerWaalsRadius);

	double static calculateOverlapVolumeAtomsWEGA(double atom1X, double atom1Y, double atom1Z,
					double atom2X, double atom2Y, double atom2Z, bool sameVanDerWaalsRadius,double radiusFixed,
					double radiusVariable);
	double static preciseCalculateOverlapVolumeAtomsWEGA(double atom1X, double atom1Y, double atom1Z,
						double atom2X, double atom2Y, double atom2Z, bool sameVanDerWaalsRadius,double radiusFixed,
						double radiusVariable);
	double static calculateOverlapVolumeAtomsWEGA2(double atom1X, double atom1Y, double atom1Z,
					double atom2X, double atom2Y, double atom2Z, bool sameVanDerWaalsRadius,double radiusFixed,
					double radiusVariable);

	
		double static calculateWeightWEGA(Atom* atom, vector<Atom>* atoms, int iAtom, bool sameVanDerWaalsRadius);
		double static calculateWeightWEGA(double atomX, double atomY, double atomZ,double sizeAtoms,double*atoms, double *radius,
				int iAtom, bool sameVanDerWaalsRadius);
	void static longestShortestAxis(Molecule& mol, double *longest, double* shortest);

	void static boundingBox(Molecule& mol, Point3DDouble* lb,
				Point3DDouble* ub);

	void static boundingBox(Molecule& mol, double margin, Point3DDouble& lb,
			Point3DDouble& ub);

	double static overlapROCS(Molecule* fixed,
				Molecule* variable);
	double static overlapWEGA(Molecule* fixed, Molecule* variable, bool sameVanDerWaalsRadius);
	double static overlapWEGA(double* fixed,unsigned int fixedSize,double *weightFixed,double* radiusFixed, double* variable,unsigned int variableSize,double*weightVariable,double * radiusVariable, bool sameVanDerWaalsRadius);
	double static preciseOverlapWEGA(double* fixed,unsigned int fixedSize,double *weightFixed,double* radiusFixed, double* variable,unsigned int variableSize,double*weightVariable,double * radiusVariable, bool sameVanDerWaalsRadius);


};

#endif /* VOLUMEOVERLAP_H */

