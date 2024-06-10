

/* 
 * File:   readMolecule.cpp
 * Author: savins
 * 
 * Created on 2 de enero de 2016, 10:50
 */

#include "ReadMolecule.h"
#include <regex>
#include <chrono>
std::istream& safeGetline(std::istream& is, std::string& t) {
	t.clear();

	

	std::istream::sentry se(is, true);
	std::streambuf* sb = is.rdbuf();

	for (;;) {
		int c = sb->sbumpc();
		switch (c) {
		case '\n':
			return is;
		case '\r':
			if (sb->sgetc() == '\n')
				sb->sbumpc();
			return is;
		case EOF:
			// Also handle the case when the last line has no line ending
			if (t.empty())
				is.setstate(std::ios::eofbit);
			return is;
		default:
			t += (char) c;
		}
	}
}

void ReadMolecule::readMol(char* filePath, Molecule *molecule,
		bool considerHydrogens, bool sameVanDerWaalsRadius) {


	ifstream infile(filePath);
	if (!infile.is_open()) {
		cerr << "uego: !! The file " << filePath << " can not be open\n";
		exit(EXIT_FAILURE);
	}
	string line = "";
	while (safeGetline(infile, line)) {
		if (line.compare("@<TRIPOS>MOLECULE") == 0) {
			//Name
			safeGetline(infile, molecule->mol_name);
			//Number variables
			safeGetline(infile, line);
			istringstream iss(line);
			iss >> molecule->num_atoms >> molecule->num_bonds
					>> molecule->num_subst >> molecule->num_feat
					>> molecule->num_sets;
			//
			safeGetline(infile, molecule->mol_type);
			safeGetline(infile, molecule->charge_type);
			safeGetline(infile, line);
			while (line.compare("@<TRIPOS>ATOM") != 0) {
				molecule->mol_comment += line;
				molecule->mol_comment += "\n";
				safeGetline(infile, line);
			}
		}

		if (line.compare("@<TRIPOS>ATOM") == 0) {
			for (long i = 0; i < molecule->num_atoms; ++i) {
				Atom *atom = new Atom();
				safeGetline(infile, line);
				istringstream iss(line);
				iss >> atom->atom_id >> atom->atom_name >> atom->x >> atom->y
						>> atom->z >> atom->atom_type >> atom->subst_id
						>> atom->subst_name >> atom->charge >> atom->status_bit;
				atom->radiusVanDerWaals = 1.80;
				molecule->atoms.push_back(*atom);
				delete atom;
			}
		}

		if (line.compare("@<TRIPOS>BOND") == 0) {
			for (long i = 0; i < molecule->num_bonds; ++i) {
				Bond *bond = new Bond();
				safeGetline(infile, line);
				istringstream iss(line);
				iss >> bond->bond_id >> bond->origin_atom_id
						>> bond->target_atom_id >> bond->bond_type
						>> bond->status_bits;

				molecule->bonds.push_back(*bond);
				delete bond;
			}
		}
	}
	infile.close();

	if (!considerHydrogens) {
		//Delete hidrogens atoms
		for (int i = molecule->num_atoms - 1; i > -1; i--) {
			//If is hidrogen
			if (molecule->atoms[i].atom_name[0] == 'H') {
				if (molecule->atoms[i].atom_name.size() > 1) {
					if (isdigit(molecule->atoms[i].atom_name[1])) {
						for (int j = molecule->num_bonds - 1; j > -1; j--) {
							if (molecule->atoms[i].atom_id
									== molecule->bonds[j].target_atom_id) {
								molecule->bonds.erase(
										molecule->bonds.begin() + j);
								break;
							}
						}
						molecule->atoms.erase(molecule->atoms.begin() + i);
						continue;
					}
				} else {
					for (int j = molecule->num_bonds - 1; j > -1; j--) {
						if (molecule->atoms[i].atom_id
								== molecule->bonds[j].target_atom_id) {
							molecule->bonds.erase(molecule->bonds.begin() + j);
							break;
						}
					}
					molecule->atoms.erase(molecule->atoms.begin() + i);
					continue;
				}
			}
		}
     	//Update numbers of atoms and bonds
		molecule->num_atoms = molecule->atoms.size();
		molecule->num_bonds = molecule->bonds.size();


		//Reasign id to bonds
		for (unsigned i = 0; i< molecule->num_atoms;i++){
			for (unsigned j = 0; j< molecule->num_bonds;j++){
				//replace origin bond
				if (molecule->bonds[j].origin_atom_id == molecule->atoms[i].atom_id){
					molecule->bonds[j].origin_atom_id = i+1;
				}
				// replace target bond
				if (molecule->bonds[j].target_atom_id == molecule->atoms[i].atom_id){
					molecule->bonds[j].target_atom_id = i+1;
				}
			}
			molecule->atoms[i].atom_id = i+1;
		}
        for (unsigned j = 0; j< molecule->num_bonds;j++){
            molecule->bonds[j].bond_id = j+1;
        }

	}

	if (!sameVanDerWaalsRadius) {
		for (int i = 0; i < molecule->num_atoms; i++) {
			molecule->atoms[i].radiusVanDerWaals =
								molecule->atoms[i].setRadiusVanDerWaals(
										molecule->atoms[i].atom_type);
		}

	}

	char msg[250];
	sprintf(msg, "File  %s read successfully", filePath);
	message(msg, MSG_INFORMATION);
}

void ReadMolecule::readDBMol2(char* filePath, vector<Molecule> *conformations,
		bool considerHydrogens, bool sameVanDerWaalsRadius) {

	ifstream infile(filePath);
	if (!infile.is_open()) {
		cerr << "uego: !! The file " << filePath << " can not be open\n";
		exit(EXIT_FAILURE);
	}

	Molecule* molecule;
	while (!infile.eof()) {

		string line = "";
		while (safeGetline(infile, line)) {
			if (line.compare("@<TRIPOS>MOLECULE") == 0) {
				molecule = new Molecule();
				//Name
				safeGetline(infile, molecule->mol_name);
				//Number variables
				safeGetline(infile, line);
				istringstream iss(line);
				iss >> molecule->num_atoms >> molecule->num_bonds
						>> molecule->num_subst >> molecule->num_feat
						>> molecule->num_sets;
				//
				safeGetline(infile, molecule->mol_type);
				safeGetline(infile, molecule->charge_type);
				safeGetline(infile, line);
				while (line.compare("@<TRIPOS>ATOM") != 0) {
					molecule->mol_comment += line + "\n";
					safeGetline(infile, line);
				}
			}

			if (line.compare("@<TRIPOS>ATOM") == 0) {

				for (long i = 0; i < molecule->num_atoms; ++i) {
					Atom atom;
					safeGetline(infile, line);
					istringstream iss(line);
					iss >> atom.atom_id >> atom.atom_name >> atom.x >> atom.y
							>> atom.z >> atom.atom_type >> atom.subst_id
							>> atom.subst_name >> atom.charge
							>> atom.status_bit;

					if (!considerHydrogens && atom.atom_name[0] == 'H') {
						if (atom.atom_name.size() > 1) {
							if (isdigit(atom.atom_name[1])) {
								continue;
							}
						} else {
							continue;
						}

					}

					if (sameVanDerWaalsRadius) {
						atom.radiusVanDerWaals = 1.80;

					} else {
						atom.radiusVanDerWaals = atom.setRadiusVanDerWaals(
								atom.atom_type);
					}
					molecule->atoms.push_back(atom);
				}
			}

			if (line.compare("@<TRIPOS>BOND") == 0) {
				for (long i = 0; i < molecule->num_bonds; ++i) {
					Bond bond;
					safeGetline(infile, line);
					istringstream iss(line);
					iss >> bond.bond_id >> bond.origin_atom_id
							>> bond.target_atom_id >> bond.bond_type
							>> bond.status_bits;
					if (!considerHydrogens) {
						bool encontrado = false;

						for (unsigned i = 0; i < molecule->atoms.size(); ++i) {
							if (molecule->atoms[i].atom_id
									== bond.target_atom_id)
								encontrado = true;
						}
						if (!encontrado) {
							continue;
						}
					}
					molecule->bonds.push_back(bond);
				}
				//Update numbers of atoms and bonds
				molecule->num_atoms = molecule->atoms.size();
				molecule->num_bonds = molecule->bonds.size();

                //Reasign id to bonds
                for (unsigned i = 0; i< molecule->num_atoms;i++){
                    for (unsigned j = 0; j< molecule->num_bonds;j++){
                        //replace origin bond
                        if (molecule->bonds[j].origin_atom_id == molecule->atoms[i].atom_id){
                            molecule->bonds[j].origin_atom_id = i+1;
                        }
                        // replace target bond
                        if (molecule->bonds[j].target_atom_id == molecule->atoms[i].atom_id){
                            molecule->bonds[j].target_atom_id = i+1;
                        }
                    }
                    molecule->atoms[i].atom_id = i+1;
                }
                for (unsigned j = 0; j< molecule->num_bonds;j++){
                    molecule->bonds[j].bond_id = j+1;
                }
				conformations->push_back(*molecule);
				delete molecule;
			}

		}
	}
	infile.close();

	char msg[250];
		sprintf(msg, "DBmolecule  %s read successfully", filePath);
		message(msg, MSG_INFORMATION);

}

