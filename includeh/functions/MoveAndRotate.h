

/*
 * File:   MoveAndRotate.h
 * Author: savins
 *
 * Created on 9 de enero de 2016, 20:47
 */

#ifndef MOVEANDROTATE_H
#define MOVEANDROTATE_H
#include "Molecule.h"
#include "Point3DDouble.h"

class MoveAndRotate {
public:
	MoveAndRotate();
	void static MolToNewPosition(Molecule* mol, double deltaX, double deltaY,
			double deltaZ);

	void static MolToNewPosition(double *atomsMolXYZ, unsigned int size, double deltaX, double deltaY,
				double deltaZ);

	void static PreciseMolToNewPosition(double *atomsMolXYZ, unsigned int size, double deltaX, double deltaY,
					double deltaZ);
	void static RotateMolAccording1Axis(const Molecule* mol, double theta,
			Point3DDouble axisA, Point3DDouble axisB, Molecule* molRotated);

	void static RotateMolAccording1Axis(const double* molAtomsXYZ,
			unsigned int nAtoms, double theta, double axisAx, double axisAy,
			double axisAz, double axisBx, double axisBy, double axisBz,
			double* molAtomsRotated);

	void static PreciseRotateMolAccording1Axis(const double* molAtomsXYZ,
				unsigned int nAtoms, double theta, double axisAx, double axisAy,
				double axisAz, double axisBx, double axisBy, double axisBz,
				double* molAtomsRotated);


	void static RotateMolAccording1Axis(const Molecule* mol, double theta,
			Point3DDouble axisA, Point3DDouble axisB, Molecule* molClockwise,
			Molecule* molAnticlockwise);
	void static RotateAtomAccording1Axis(Atom* atomClockwise,
			Atom* atomAnticlockwise, double theta, Point3DDouble axisA,
			Point3DDouble axisB);

	virtual ~MoveAndRotate();
private:

};

#endif /* MOVEANDROTATE_H */

