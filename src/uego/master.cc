#include <string.h>
#include "uego.h"
using namespace std;
////////////////////////////////////////////////////////////
// $Id: master.cc,v 2.7 1998/04/01 21:05:42 jelasity Exp $
// master.cc
// defines class Master;
////////////////////////////////////////////////////////////
// modification history:
//	Jelasity 98 01 24 new save flag
////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------

void Master::ReInit(Ini* ini, char* trace) {

	SearchSpElement *root;
	FunctionEvals = 0;
	FailFlag = 1 == 1;

	_ini = ini;		// only storing pointer!
	tracename = trace;	// only storing pointer!

	// --- initializing species list with root:
	// --- 1 random element from space
	head = new SpeciesList;
	if (head != NULL) {
		root = _ini->Prototype()->RandNew();
	}
	if (root != NULL) {
		head->next = new SpeciesList(root, 0);
	}
	if (head == NULL || root == NULL || head->next == NULL) {
		message("No memory for master.", MSG_ERROR);
		return;
	};

	
	// --- Creation of new species
	// --- Original position and angle
	SearchSpElement *allZero;
	allZero = _ini->Prototype()->RandNew();
	double* y = new double[ini->Dimension()];
	for (int i = 0; i < ini->Dimension(); i++) {
		y[i] = 0;
	}
	y[1] = 1;
	allZero->SetX(y);
	allZero->UpdateValue();

	//cout << "Rotate Y: " << allZero->CurrValue() << endl;
	head->next->prev = head;
	head->next->next = new SpeciesList(allZero, 0);
	head->next->next->prev = head->next;
	// --- Rotated 180 degres around X axis
	
	int cantidad = 3, angulos = 2, temp;
	double p1x, p1y, p1z;
	double dosPI = 6.283185;
	double tempAngulo;
	SearchSpElement *specie;
	int moleculas[] = { 1, 2, 4 };
	int longitud = (sizeof(moleculas) / sizeof(*moleculas));
	for (int i = 0; i < cantidad; i++) {
		temp = moleculas[i];
		p1x = temp % 2;
		temp = temp / 2;
		p1y = temp % 2;
		temp = temp / 2;
		p1z = temp % 2;
		//cout << "X: " << p1x << " Y: " << p1y << " Z: " << p1z << endl;
		for (int j = 1; j < angulos; j++) {
			tempAngulo = (dosPI / angulos) * j;
			//cout << "angulo: " << i << ": " << tempAngulo << endl;
			specie = _ini->Prototype()->RandNew();
			y[0] = tempAngulo;
			y[1] = p1x;
			y[2] = p1y;
			y[3] = p1z;
			specie->SetX(y);
			specie->UpdateValue();
			//cout << "Specie: " << j << ": " << specie->CurrValue() << endl;
			SpeciesList *tmp;
			tmp = head->next;
			for (; tmp->next != NULL; tmp = tmp->next) {
			}
			tmp->next = new SpeciesList(specie, 0);
			tmp->next->prev = tmp;

		}
	}
	delete[] y;
	length = 5;	//(cantidad - 1) * (angulos - 1) + 2;
	//FunctionEvals += 7;	//length * 2 + 3;
	level = 0;
	FailFlag = 1 == 0;
}
;

// -----------------------------------------------------------------------

void Master::Clean() {

	SpeciesList *tmp;

	if (head != NULL) {
		while (head->next != NULL) {
			tmp = head->next;
			head->next = head->next->next;
			delete tmp;
		};
		delete head;
	};

	// _ini and tracename must be deleted by the caller of the constructor
}
;

// -----------------------------------------------------------------------

void Master::Fuse() {

	const double r = _ini->R(level);
	SpeciesList *tmp, *act;

	if (length < 5)
		return;

	for (act = head->next->next; act != NULL; act = act->next) {
		tmp = act->prev;
		while (tmp != head && length > 5) {

			// --- compare act and tmp for fusion

			if (act->center->Distance(tmp->center) < r) {

				if (act->level <= tmp->level) {
					// --- absorbing tmp
					act->Absorb(tmp);

					// --- cutting out tmp
					// (tmp is never the last!)
					length--;
					tmp = tmp->prev;
					tmp->next = tmp->next->next;
					delete tmp->next->prev;
					tmp->next->prev = tmp;
				} else {
					// --- absorbing act
					tmp->Absorb(act);

					// --- cutting out act
					length--;
					tmp = act; // old tmp not used anymore
					act = act->prev;
					act->next = act->next->next;
					delete tmp;
					if (act->next != NULL)
						act->next->prev = act;
					break;
				};
			} else
				tmp = tmp->prev; // nothing has been cut out
		};
	};
}
;

// -----------------------------------------------------------------------

void Master::_NewSpecies(long evals) {

	SpeciesList newspechead,	// First element is newspechead.next!
			*newlst, *newend,	// shows end of new spec list
			*tmp;		// position in old list
	long newevals;

	FailFlag = 1 == 1;

	newend = &newspechead;
	for (tmp = head; tmp->next != NULL; tmp = tmp->next) {
		// --- to allow multiple calls w.o. level increased
		if (tmp->next->level == level)
			continue;

		// --- list of new species in newlst
		newevals = tmp->next->NewSpecies(&newlst, level, evals);
		FunctionEvals += newevals;
		if (tmp->next->Fail())
			return;

		// --- insert new list to newspechead, increase length
		newend->next = newlst;
		if (newlst != NULL)
			newlst->prev = newend;
		while (newend->next != NULL) {
			++length;
			newend = newend->next;

		};
	};
	tmp->next = newspechead.next;
	if (newspechead.next != NULL)
		newspechead.next->prev = tmp;
	newspechead.next = NULL; // to prevent destructing;

	FailFlag = 1 == 0;
}
;

// -----------------------------------------------------------------------

void Master::_Mutate(long nOfMutations) {

	SpeciesList newspechead,	// First element is newspechead.next!
			*newlst, *newend,	// shows end of new spec list
			*tmp;		// position in old list
	long newevals;

	FailFlag = 1 == 1;

	newend = &newspechead;
	for (tmp = head; tmp->next != NULL; tmp = tmp->next) {
		// --- to allow multiple calls w.o. level increased
		if (tmp->next->level == level)
			continue;

		// --- list of new species in newlst

		newevals = tmp->next->Mutate(&newlst, level, nOfMutations);
		FunctionEvals += newevals;
		if (tmp->next->Fail())
			return;

		// --- insert new list to newspechead, increase length
		newend->next = newlst;
		if (newlst != NULL)
			newlst->prev = newend;
		while (newend->next != NULL) {
			++length;
			newend = newend->next;

		};
	};
	tmp->next = newspechead.next;
	if (newspechead.next != NULL)
		newspechead.next->prev = tmp;
	newspechead.next = NULL; // to prevent destructing;

	FailFlag = 1 == 0;
}
;

// -----------------------------------------------------------------------

void Master::NewSpecies() {

	const long oldlength = length;
	SpeciesList *tmp;

	//printf("Number of function evaluation %i \n", INI.NewSpecEvals(level));
	// cout << "Level: "<<level<<endl;
	// cout << "Length: "<<length;
	_NewSpecies(_ini->NewSpecEvals(level) / length);
//	printf(" Length 1 = %i \n", length);

	// ---------------------------------------------------
	// --- beginning of species creation forcing extension
	// ---------------------------------------------------
	if (Fail())
		return;

	/*const long needed_msn = (long) pow(M(_ini->Levels() - 1),
	 (double) level / (_ini->Levels() - 1)), maxevals = head->Evals(
	 level) / oldlength;
	 long max_spec_num = (long) M(level);

	 Fuse();
	 if (Fail() || maxevals < 3)
	 return;
	 //printf("Number of function evaluation2 %i \n", _ini->NewSpecEvals(level) / length );

	 _NewSpecies(maxevals);*/

	//printf(" Length 2 = %i \n", length);
	/*if (Fail())
	 return;
	 Fuse();
	 if (Fail())
	 return;

	 --max_spec_num;

	 CheckLength(max_spec_num);*/

	// ---------------------------------------------------
	// --- end of species creation forcing extension
	// ---------------------------------------------------
}
;

// -----------------------------------------------------------------------

void Master::Mutate() {

	_Mutate(5);

	if (Fail())
		return;

}
;

// -----------------------------------------------------------------------

double Master::M(long i, Ini* ini) {

	return ini->MaxSpecNumber();
}
;

// -----------------------------------------------------------------------

void Master::Optimize() {

	SpeciesList *tmp;
	long newevals;

	FailFlag = 1 == 1;

	for (tmp = head->next; tmp != NULL; tmp = tmp->next) {
		newevals = tmp->Optimize();

		FunctionEvals += newevals;
		if (tmp->Fail())
			return;
	};

	FailFlag = 1 == 0;
}
;

// -----------------------------------------------------------------------

void Master::ElitistSelection(long new_length) {
	SpeciesList *tmp;
	SearchSpElement * root;

	SpeciesList *end;

	root = _ini->Prototype()->RandNew();

	for (long i = 0; i < length; i++) {
		tmp = head->next;
		while (tmp->next != NULL) {

			if (tmp->center->CurrValue() < tmp->next->center->CurrValue()) {
				// --- sort
				root->UpdateFrom(tmp->center);
				tmp->center->UpdateFrom(tmp->next->center);
				tmp->next->center->UpdateFrom(root);

			}	//end if
			tmp = tmp->next;
		};	//end while
	}
	if (new_length == -1)
		new_length = _ini->MaxSpecNumber();
	if (length > new_length) {
		message("Too many species, shortening species list.",
		MSG_INFORMATION);
		end = head;
		while (end->next != NULL)
			end = end->next;

		// --- deleting from end of list
		for (; length > new_length; --length) {
			end = end->prev;
			delete end->next;
		};
		end->next = NULL;
	};
}
;

// ----------------------------------------------------------------------
void Master::CheckLength(long new_length) {

	SpeciesList *end;

	if (new_length == -1)
		new_length = _ini->MaxSpecNumber();
	if (length > new_length) {
		message("Too many species, shortening species list.",
		MSG_INFORMATION);
		end = head;
		while (end->next != NULL)
			end = end->next;

		// --- deleting from end of list
		for (; length > new_length; --length) {
			end = end->prev;
			delete end->next;
		};
		end->next = NULL;
	};
}
;

// -----------------------------------------------------------------------

void Master::PrintList() {

	SpeciesList *tmp;
	double *xaux = new double[INI.Dimension()];

	tmp = head->next;
	while (tmp != NULL) {

		tmp->center->GetX(xaux);
		for (int i = 0; i < INI.Dimension(); i++)
			printf(" %lf  ", xaux[i]);
		printf("    %lf \n", tmp->center->CurrValue());
		tmp = tmp->next;

	}

}
// -----------------------------------------------------------------------

void Master::SaveListFile(int level, string file, string step) {

	char msg[500];
	string path = INI.getPathOutputFiles();
	path.append(file);

	/*if (level == 0 || level == 1) {
	 remove(path.c_str());
	 }*/
	ofstream fs(path.c_str(), std::fstream::app | std::fstream::in);

	// con level -1 pongo mensajes especiales como por ejemplo
	// el nombre de las moleculas de entrada
	if (level == -1) {
		fs << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
		fs << "@ Moleculas: \n";
		fs << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
		fs << "Molecula Query: " << INI.getPathMolQuery() << "\n";
		fs << "Molecula Variable " << INI.getPathMolVariable() << "\n";
		fs.close();
		sprintf(msg, "Writed file '%s'.", file.c_str());
		message(msg, MSG_INFORMATION);
		return;
	}
	fs << "##########################" << endl;
	fs << "Level: " << level << " " << step << endl;
	fs << "##########################" << endl;
	SpeciesList *tmp;
	double *xaux = new double[INI.Dimension()];
	tmp = head->next;
	while (tmp != NULL) {

		double *normx = xaux;

		tmp->center->GetX(xaux);
		for (long i = 0; i < INI.Dimension(); ++i) {
			xaux[i] = normx[i] * (INI.Upb(i) - INI.Lowb(i)) + INI.Lowb(i);
		};
		for (int i = 0; i < INI.Dimension(); i++)
			fs << xaux[i] << " ";
		//printf(" %lf  ", xaux[i]);
		fs << tmp->center->CurrValue() << endl;
		//printf("    %lf \n", tmp->center->CurrValue());

		tmp = tmp->next;
		xaux = normx;

	}

	fs.close();
	sprintf(msg, "Writed file '%s'.", file.c_str());
	message(msg, MSG_INFORMATION);

}
// -----------------------------------------------------------------------

void Master::PlotMoleculesList(int level, string step) {

	// Iterar por la lista de soluciones
	SpeciesList *tmp;
	double *xaux = new double[INI.Dimension()];
	tmp = head->next;

	int cont = 0;
	while (tmp != NULL) {

		double *normx = xaux;

		tmp->center->GetX(xaux);
		for (long i = 0; i < INI.Dimension(); ++i) {
			xaux[i] = normx[i] * (INI.Upb(i) - INI.Lowb(i)) + INI.Lowb(i);
		};

		string name = to_string(level) + "-"+ step+ "-"+to_string(cont);

		Plot::MoleculesSolution(INI.getMolQuery(), INI.getMolVariable(),
				xaux[0], xaux[1], xaux[2], xaux[3], xaux[4], xaux[5], xaux[6],
				xaux[7], xaux[8], xaux[9], tmp->center->CurrValue(), name);

		tmp = tmp->next;
		xaux = normx;
		cont ++;
	}

}
// -----------------------------------------------------------------------
void Master::_Go() {
	string nombreArchivo = "evolucion.txt";
	//bool guardarMoleculasMol2 = true;
	SaveListFile(-1, nombreArchivo, "Inicio");
	//BORRAR
	/*for (int i = 0; i < INI.Levels(); i++) {
	 cout << "Level: " << i << endl;
	 cout << "Evals:" << INI.Evals(i) << endl;
	 cout << "NewEvals: " << INI.NewSpecEvals(i) << endl;
	 }*/

	//FunctionEvals += head->next->Optimize(_ini->Evals(0));
	//getBestSpecies(0, guardarMoleculasMol2);
	//getSpeciesList(0, guardarMoleculasMol2);
	SaveListFile(0, nombreArchivo, "Inicio");
	//PlotMoleculesList(0, "Initial");
	Optimize();
	//SaveListFile(0, nombreArchivo, "Optimize Inicial");


	for (level = 1; level < _ini->Levels(); ++level) {
		//getSpeciesList(level, guardarMoleculasMol2);

		NewSpecies();
		//PlotMoleculesList(level, "NewSpecies");
		if (Fail())
			return;
		//SaveListFile(level, nombreArchivo, "1NewSpecies");
		//getSpeciesList(level, guardarMoleculasMol2);

		//Mutate();
		//SaveListFile(level, nombreArchivo, "Mutate");

		ElitistSelection();
		//SaveListFile(level, nombreArchivo, "2ElitistSelection");
		//getSpeciesList(level, guardarMoleculasMol2);

		//Fuse();
		//getSpeciesList(level, guardarMoleculasMol2);
		/*if (Fail())
		 return;
		 CheckLength();*/

		/*ElitistSelection();
		 SaveListFile(level, nombreArchivo, "Elitist");
		 */
		Optimize();
		if (Fail())
			return;
		//getSpeciesList(level, guardarMoleculasMol2);
		//SaveListFile(level, nombreArchivo, "3Optimize");

		/*Fuse();
		 if (Fail())
		 return;*/
		//SaveListFile(level, nombreArchivo, "Optimize");
		//getBestSpecies(level, guardarMoleculasMol2);
		//getSpeciesList(level, guardarMoleculasMol2);
	};
}
;

// -----------------------------------------------------------------------

void Master::Save(FILE* stream, double tiempo, unsigned long flags) {

	SpeciesList *tmp, *tmp_best;
	double best, best_x0, best_x1;

	FailFlag = 1 == 1;

	if (flags & SAVE_INI) {
		_ini->Save(stream);
		if (_ini->Fail())
			return;
	};

	if (fprintf(stream, "%s %s %ld %ld %lf ",
			INI.getMolQuery()->mol_name.c_str(),
			INI.getMolVariable()->mol_name.c_str(), FunctionEvals, length,
			tiempo) == EOF) {
		message("Error saving master.", MSG_ERROR);
		return;
	};

// --- which is best?
	tmp = head->next;
	tmp_best = head->next;
	tmp->center->PreciseUpdateValue();
	best = tmp->center->CurrValue();
	for (; tmp != NULL; tmp = tmp->next) {

		tmp->center->PreciseUpdateValue();

		if (tmp->center->CurrValue() > best) {
			best = tmp->center->CurrValue();
			tmp_best = tmp;
		}
	}
	tmp_best->Save(stream, 0 == 0);

	if (tmp_best->Fail())
		return;

// --- saving species
	if (!(flags & SHORT_SAVE)) {
		tmp = head->next;
		while (tmp != NULL) {
			tmp->Save(stream);

			if (tmp->Fail())
				return;
			tmp = tmp->next;
		};
	};

	FailFlag = 1 == 0;
}
;

// -----------------------------------------------------------------------

void Master::CheckPoint() {

	char name[500], msg[500];
	FILE* outf;

	FailFlag = 1 == 0;
	if (tracename == NULL)
		return;
	else
		FailFlag = 1 == 1;

	strcpy(name, tracename);
	sprintf(name + strlen(name), "%04ld.ckp", tracenum);
	if ((outf = fopen(name, "wt")) == NULL) {
		sprintf(msg, "Could not open file '%s'.", name);
		message(msg, MSG_ERROR);
		return;
	};

	Save(outf); // Sets FailFlag
	fclose(outf);

	++tracenum;
}
;
/**
 * Get the best species in this level and can save in a mol2 file the best molecule.
 */
void Master::getBestSpecies(short level, bool saveFile) {
	SpeciesList *tmp, *tmp_best;
	double best;
	// --- which is best?
	tmp = head->next;
	tmp_best = head->next;
	best = tmp->center->CurrValue();
	for (; tmp != NULL; tmp = tmp->next)
		if (tmp->center->CurrValue() > best) {
			best = tmp->center->CurrValue();
			tmp_best = tmp;
		}
	double *tempx = new double[INI.Dimension()];
	tmp_best->center->GetX(tempx);

	printf("Level: %hu\n", level);
	for (int i = 0; i < INI.Dimension(); i++) {
		printf("%lf ", tempx[i]);
	}
	printf("Value: %lf \n\n", tmp_best->center->CurrValue());

	if (saveFile) {
		double *xaux = new double[INI.Dimension()];
		double *normx = xaux;

		tmp_best->center->GetX(xaux);
		for (long i = 0; i < INI.Dimension(); ++i) {
			xaux[i] = normx[i] * (INI.Upb(i) - INI.Lowb(i)) + INI.Lowb(i);
		};
		//Print Molecule Best Solution
		Molecule * moleculeRotated = new Molecule();
		Point3DDouble p1, p2;
		p1.x = xaux[1];
		p1.y = xaux[2];
		p1.z = xaux[3];
		p2.x = xaux[4];
		p2.y = xaux[5];
		p2.z = xaux[6];

		MoveAndRotate::RotateMolAccording1Axis(INI.getMolVariable(), xaux[0],
				p1, p2, moleculeRotated);
		MoveAndRotate::MolToNewPosition(moleculeRotated, xaux[7], xaux[8],
				xaux[9]);
		string pathMolecule = INI.getPathOutputFiles();

		std::stringstream sstm;
		sstm << "BestSolLvl-" << level << ".mol2";
		string result = sstm.str();

		pathMolecule.append(result);
		WriteMolecule::writeMol(pathMolecule, moleculeRotated);
		xaux = normx;
	}

}

void Master::getSpeciesList(short level, bool saveFile) {
	SpeciesList *tmp;
	tmp = head->next;
	int contador = 0;
	for (; tmp != NULL; tmp = tmp->next) {
		double *xaux = new double[INI.Dimension()];
		double *normx = xaux;

		tmp->center->GetX(xaux);

		cout << contador << "--> Currvalue: " << ": "
				<< tmp->center->CurrValue() << endl;

		for (long i = 0; i < INI.Dimension(); ++i) {
			//cout << xaux[i] << ", ";
			xaux[i] = normx[i] * (INI.Upb(i) - INI.Lowb(i)) + INI.Lowb(i);
			cout << xaux[i] << ", ";
		};
		cout << endl << endl;
		if (saveFile) {
			//Print Molecule Best Solution
			Molecule * moleculeRotated = new Molecule();
			Point3DDouble p1, p2;
			p1.x = xaux[1];
			p1.y = xaux[2];
			p1.z = xaux[3];
			p2.x = xaux[4];
			p2.y = xaux[5];
			p2.z = xaux[6];

			MoveAndRotate::RotateMolAccording1Axis(INI.getMolVariable(),
					xaux[0], p1, p2, moleculeRotated);
			MoveAndRotate::MolToNewPosition(moleculeRotated, xaux[7], xaux[8],
					xaux[9]);
			string pathMolecule = INI.getPathOutputFiles();

			std::stringstream sstm;
			sstm << "Especie-" << level << "-" << contador << ".mol2";

			string result = sstm.str();

			pathMolecule.append(result);
			WriteMolecule::writeMol(pathMolecule, moleculeRotated);
		}
		contador += 1;
		xaux = normx;

	}

}

double Master::getBestSpecies() {
	SpeciesList *tmp, *tmp_best;
	double best;
	// --- which is best?
	tmp = head->next;
	tmp_best = head->next;
	best = tmp->center->CurrValue();
	for (; tmp != NULL; tmp = tmp->next)
		if (tmp->center->CurrValue() > best) {
			best = tmp->center->CurrValue();
			tmp_best = tmp;
		}
	return best;
}

//------------------------------------------

