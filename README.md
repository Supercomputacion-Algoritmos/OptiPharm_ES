# OptiPharm_ES

Source code of OptiPharm_ES under Mozilla Public License Version 2.0, https://www.mozilla.org/en-US/MPL/2.0/  
For non-academics: a licence needed. Contact with the authors.

Eigen and Openeye Libraries belong to their authors and access to them depends on any applicable restrictions.
Additionally, a valid openeye license is mandatory to run the software.

For more information about libraries go to http://eigen.tuxfamily.org/ and https://www.eyesopen.com/.

# How to use

OptiPharm_ES is a parameterizable evolutionary algorithm. The complete set of parameters is shown in the
executable itself by means of the instruction:

```
./OptiPharm_ES
```
The above instruction shows a help text similar to the following:

```
./executable -c <ext> -q <query.mol2> -d <database.mol2> [-o ./] [-h 0] [-
w 1] [-N <>] [-M <>] [-L <>] [-R <>] [-pre 1] [-pz 0] [-smol] [-fvdwr
<file>] [-lowL 0.00] [-S]
```
```
The parameters:
-c: extension of configuration file.
-q: path of query molecule.
-d: path of database with molecule or multiple molecules being
compared with the query.
-o: path of outputFiles save by the algorithm. Default: ./
-h: considers atoms of hydrogen. Default: 0 (no).
-w: set the same van der Waals radius to all the atoms. Default: 1
(yes).
-N: max. number of evaluations (int).
-M: max. number of species (int).
-L: levels (int).
-R: smallest radius (double).
-pre: preprocess molecule before with prePCA.py. Default: 0 (no).
-pz: set P2 as 0,0,0. Default: 0 (no).
-smol: save solution in mol2 files. Default: 0 (no).
-fvdwr: indicate the file with van der Waals radius.
-lowL: lower limit that we considered to execute the algorithm.
-S: only first solution is shown.
```
Each parameter is explained in further detail below:

```
-c: Path of the configuration file. It has information about the four input parameters.
-q: Path of a query molecule in mol2 format.
-d: Path of a database, which can include one or many compounds, in mol2 format. They will be
compared with the query compound iteratively.
-o: The output folder, where the output files are saved, i.e. file containing the final solution of the
compounds. The output directory must exist.
-h: Hydrogen atoms can be ignored or included from the compounds. Possible values: 0 or 1. By default,
it is set to 0 and hydrogen atoms are not considered.
-w: Van der Waals radius can be set to the same value for all periodic elements, or it can have a different
value for each kind of atom. Possible values: 0 or 1. By default, a 0 value is set what means that all the
```

```
atoms are considered to have the same radius value.
-N: Set the maximum number of evaluations.
-M: Set the maximum number of species.
-L: Set the number of levels or iterations that OptiPharm_ES performs.
-R: Set the smallest radius that a species can have.
-pre: A external script can be executed before the optimization starts to align the centroids of the
compounds at the centre of coordinates. Possible values: 0 or 1. By default, the pre-process is considerd,
the value is 1.
-pz: OptiPharm_ES can generate the two 3D points of an axis randomly or can fix one of them at (0,0,0).
By default, points are generated randoml. Possible values: 0 or 1. By default, the parameter is set to 0, a
random position for two axis points is possible.
-smol: Three files are saved in mol2 format: the two original input compounds and the best solution.
-fvdwr: The radius of each element is written in order.
-lowL: A filter can be applied to reduce computational cost when many comparisons have to be run. The
size of both compounds is evaluated and if the value of that comparison is lower than the limit set, the
optimization does not start. This limit must be a value between 0 and 1.
-S: OptiPharm_ES by default shows all M solutions. However, by using this parameter, only the first
solution (and the best of all) is shown.
```
## Examples

```
The basic execution command is as follows. It is necessary to indicate the name of the executable, the
extension of the configuration file and the two compounds.
```
```
./OptiPharm_ES -c FMOL -q compound1.mol2 -d compound2.mol
```
With the exception of these 3 parameters, other parameters are individual and independent and can be used as
desired. Some examples are shown below. More than one parameter may appear in the examples.

```
Adding the -S parameter will only display the single best result:
```
```
./OptiPharm_ES -c FMOL -q compound1.mol2 -d compound2.mol2 -S
```
```
Two options exist to change the configuration file: 1. Changing the parameters without performing any
optimization: ./OptiPharm_ES -c FMOL -N 100000 -M 10 -L 7 -R 1 2. If the compounds are
specified, then the configuration would be modified and the optimization would be performed later.
./OptiPharm_ES -c FMOL -q compound1.mol2 -d compound2.mol2 -N 100000 -M 10
-L 7 -R 1
It is possible to indicate whether or not you want to consider hydrogen and set the van der Waals radius of
all elements to 1.8. Or, on the contrary, each element can have its own corresponding radius.
```
```
./OptiPharm_ES -c FMOL -q compound1.mol2 -d compound2.mol2 -h 1 -w 0
```

```
If you want to save the final files, add the command -smol 1:
```
```
./OptiPharm_ES -c FMOL -q compound1.mol2 -d compound2.mol2 -smol 1
```
```
Default van der Waals radius can be modified by an external file. Note that it is necessary to have
parameter -w 1 indicated so that each atom has its own value.
```
```
./OptiPharm_ES -c FMOL -q compound1.mol2 -d compound2.mol2 -fvdwr file.txt
```
# Output

A standard output is shown below:

```
optipharm: -- Opening ini file OPini.FMOL
optipharm: -- File fda1081.mol2 read successfully
optipharm: -- DBmolecule fda1040.mol2 read successfully
optipharm: -- Reading ini file.
optipharm: -- Creating master.
optipharm: -- Starting optimalization.
optipharm: -- OPini.FMOL
optipharm: -- Too many species, shortening species list.
optipharm: -- Too many species, shortening species list.
optipharm: -- Too many species, shortening species list.
optipharm: -- Too many species, shortening species list.
optipharm: -- Saving results.
9592 5 0.010037 1.6639323194e+00 4.8040692111e-01 9.2650620205e-
-4.1692474806e-02 -5.1100389006e-01 -1.7030956578e-02 2.9543437633e-
-4.7816938633e-01 -9.8099345724e-02 1.2674744738e-01 7.2769969420e-
8.3208150222e-
2.9021873564e+00 -2.8010248969e-01 4.9754825833e-01 -3.6159493233e-
-1.2740793305e-01 -7.1099508601e-02 3.5540240000e-01 9.0128908453e-
-2.2500812431e-01 -3.5501629855e-01 8.0788557779e-
3.1653442033e+00 -8.1699949960e-02 1.8726541447e-01 3.4187031000e-
-7.9574913342e-02 -1.5254234876e-02 5.2080494161e-02 -5.8560735206e-
7.3270865432e-02 -8.4514257290e-02 8.0625935220e-
1.6639323194e+00 4.8040692111e-01 9.2650620205e-02 -4.1692474806e-
-5.1100389006e-01 -1.7030956578e-02 2.9543437633e-02 -4.7816938633e-
-9.8099345724e-02 1.2674744738e-01 8.3208150222e-
1.4356885235e+00 1.9640549467e+00 2.4252595183e-01 2.0080135009e-
9.6632857434e-01 1.7720855238e-01 2.1740626501e-01 -5.2624706605e-
-6.4444753579e-03 -2.1452780507e-01 8.2760432019e-
1.1312333688e+00 1.1662157476e+00 1.0737602789e+00 3.0629038436e-
1.4871887682e+00 1.2110302535e+00 2.5019990127e-01 -2.4795567958e-
-5.7356382833e-01 1.0083775873e-01 8.0723945432e-
optipharm: -- Success.
```

After message "optipharm: -- Saving results." the value of each solution is shown. The first line shows the
following:

```
[evaluation total number] [final species number in the list] [time in
seconds] [angle] [x-coordinate of P1] [y-coordinate of P1] [z-coordinate
of P1] [x-coordinate of P2] [y-coordinate of P2] [z-coordinate of P2] [x-
axis translation] [y-axis translation] [z-axis translation] [initial
tanimoto score] [final tanimoto score]
```
The rest of the lines show the same information but without the first three field or the initial tanimoto score:

```
[angle] [x-coordinate of P1] [y-coordinate of P1] [z-coordinate of P1] [x-
coordinate of P2] [y-coordinate of P2] [z-coordinate of P2] [x-axis
translation] [y-axis translation] [z-axis translation] [final tanimoto
score]
```

