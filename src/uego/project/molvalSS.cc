#include "uego.h"
#include "time.h"
//#include "internal.h"
#include <stdlib.h>
#include <unistd.h>
#include "Tanimoto.h"
#include <iomanip>
////////////////////////////////////////////////////////////
// $Id: testval.cc,v 2.6 1998/03/29 10:40:32 jelasity Exp $
// testval.cc
// definition of NDimRealElement::Value()
// objective function library for n dimensional real spaces
// this version was originaly created for experimental tests
////////////////////////////////////////////////////////////
// modification history:
////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------
#include "VolumeOverlap.h"
#include "MoveAndRotate.h"
#include "Tanimoto.h"
#include <iostream>


double NDimMolElement::Value() {
	//time_t t1,t2;


	//t1=time(NULL);
	if (x[1] == x[4] && x[2] == x[5] && x[3] == x[6]) {
		return 0;
	}

	double * newAtomsRotated =&(INI.getMolVariable()->getAtomsXYZ()[INI.getMolVariable()->atoms.size()*3]);
	MoveAndRotate::RotateMolAccording1Axis(INI.getMolVariable()->getAtomsXYZ(),
	INI.getMolVariable()->atoms.size(), x[0], x[1], x[2], x[3], x[4], x[5],
			x[6], newAtomsRotated);

	MoveAndRotate::MolToNewPosition(newAtomsRotated,
	INI.getMolVariable()->atoms.size(), x[7], x[8], x[9]);





	double VAB = VolumeOverlap::overlapWEGA(
	INI.getMolQuery()->getAtomsXYZ(),
	INI.getMolQuery()->atoms.size(), INI.getMolQuery()->getWeightAtoms(),
	INI.getMolQuery()->getRadiusAtoms(), newAtomsRotated,
	INI.getMolVariable()->atoms.size(),
	INI.getMolVariable()->getWeightAtoms(),
	INI.getMolVariable()->getRadiusAtoms(),
	INI.getSameVanDerWaalsRadius());


double result = Tanimoto::calculateTanimotoGeneric(
	INI.getMolQuery()->tanimoto,
	INI.getMolVariable()->tanimoto, VAB);


	return result;


}
;


double NDimMolElement::PreciseValue() {
	//time_t t1,t2;

	//t1=time(NULL);
	if (x[1] == x[4] && x[2] == x[5] && x[3] == x[6]) {
		return 0;
	}
	
	double * newAtomsRotated =&(INI.getMolVariable()->getAtomsXYZ()[INI.getMolVariable()->atoms.size()*3]);
	MoveAndRotate::PreciseRotateMolAccording1Axis(INI.getMolVariable()->getAtomsXYZ(),
	INI.getMolVariable()->atoms.size(), x[0], x[1], x[2], x[3], x[4], x[5],
			x[6], newAtomsRotated);

	MoveAndRotate::PreciseMolToNewPosition(newAtomsRotated,
	INI.getMolVariable()->atoms.size(), x[7], x[8], x[9]);




	double VAB = VolumeOverlap::preciseOverlapWEGA(
	INI.getMolQuery()->getAtomsXYZ(),
	INI.getMolQuery()->atoms.size(), INI.getMolQuery()->getWeightAtoms(),
	INI.getMolQuery()->getRadiusAtoms(), newAtomsRotated,
	INI.getMolVariable()->atoms.size(),
	INI.getMolVariable()->getWeightAtoms(),
	INI.getMolVariable()->getRadiusAtoms(),
	INI.getSameVanDerWaalsRadius());


double result = Tanimoto::calculateTanimotoGeneric(
	INI.getMolQuery()->tanimoto,
	INI.getMolVariable()->tanimoto, VAB);
	

	return result;

}
;

