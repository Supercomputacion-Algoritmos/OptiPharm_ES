/*
 * File:   Molecule.h
 * Author: Savíns
 *
 * Created on 25 de noviembre de 2015, 16:26
 */

#ifndef MOLECULE_H
#define MOLECULE_H

#include <string>
#include <vector>
#include "Atom.h"
#include "Bond.h"

using namespace std;

class Molecule {
private:
	double * atomsXYZ;
	double * radiusAtoms;
	double * weightAtoms;
public:
	string mol_name, mol_type, charge_type, mol_comment, name_file;
	int num_atoms, num_bonds, num_subst, num_feat, num_sets;
	vector<Atom> atoms;

	vector<Bond> bonds;

	double tanimoto;

	Molecule();
	Molecule(string mol_name, string mol_type, string charge_type,
			string mol_comment, int num_atoms, int num_bonds, int num_subst,
			int num_feat, int num_sets);
	Molecule(const Molecule &mol);
	void setTanimoto(double* atomsXYZ,  unsigned int size,double *weight, double*radius, bool sameVDWR);
	virtual ~Molecule();
	string toString();

	void ObjectToArray(double* atoms, double* radius, double* weights,
			bool sameVanderWaalsRadius);


	void calculateWeightWEGAAtoms(double * atoms, unsigned int nAtoms,
			double * weights, double*radius, bool sameVanderWaalsRadius);

	void setArraysAtoms(unsigned int size) {
		this->atomsXYZ = new double[size * 6];
		this->radiusAtoms = new double[size];
		this->weightAtoms = new double[size];
	}
	double * getAtomsXYZ(){
		return this->atomsXYZ;
	}

	double * getRadiusAtoms(){
		return this->radiusAtoms;
	}
	double * getWeightAtoms(){
		return this->weightAtoms;
	}

	bool updateAtomCoordinates(double * coordinates);

};

#endif /* MOLECULE_H */

