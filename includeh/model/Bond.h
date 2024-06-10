
/* 
 * File:   Bond.h
 * Author: Savíns
 *
 * Created on 25 de noviembre de 2015, 16:27
 */

#ifndef BOND_H
#define BOND_H
#include <string>
#include <sstream>
#include <unordered_map>
#include <iostream>
#include <fstream>
using namespace std;

class Bond {
public:
	int bond_id, origin_atom_id, target_atom_id;
	string bond_type, status_bits;
	Bond();
	Bond(int bond_id, int origin_atom_id, int target_atom_id, string bond_type,
			string status_bits);

	virtual ~Bond();
	string toString();
};

#endif /* BOND_H */

