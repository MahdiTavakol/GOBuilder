/*
* =============================================================================
*
* Builds Graphene Oxide from the xyz file of the graphene
*
* Author:  Mahdi Tavakol (mahditavakol90@gmail.com)
* Version: 09.25.2016
*
* Compile: g++ -stc=c++11 -o GO2 GO2.cpp
* Call:    ./GO2 [name of input file (.xyz)] [name of output file (.xyz)] [Percentage of Epoxy groups] [percentage of Hydoxyl groups]
* [percentage of Carboxyl groups]
*
* =============================================================================
*/


#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include <ctime>
#include <iomanip>
#include <random>
#include "Atoms.h"
using namespace std;


static std::random_device rd; // random device engine, usually based on /dev/random on UNIX-like systems
							  // initialize Mersennes' twister using rd to generate the seed
static std::mt19937 rng(rd());

int up_or_down();
int get_event(float , float );
int writeHydroxyl(std::ofstream&, int , Atom& , int, int );
int writeCarboxyl(std::ofstream&, int , int , Atom[] , bool );
int writeHydrogen(std::ofstream&, int, int, Atom[]);
float distance(Atom, Atom);
int selectEpoxy(Atom[], int, int);
int writeEpoxy(std::ofstream& , Atom[], int , int, int, int );
void summaryOfAtomTypes(int, int);




int main()
{
	float EpoxyPercent    = 10.0f;
	float HydroxylPercent = 30.0f;
	float CarboxylPercent = 0.0f;
	int GOType          = 0;  // Type 1 Epoxy Covered GO, Type 2 Hydroxyl Covered GO, Type 3 Epoxy-Hydroxyl Covered GO
	stringstream eStream, hStream;
	eStream << EpoxyPercent;
	hStream << HydroxylPercent;
	string eString, hString, outputFileName, dummy, atomCountString;
	eString = eStream.str();
	hString = hStream.str();
	if      ( EpoxyPercent && !HydroxylPercent) GOType = 1;
	else if (!EpoxyPercent &&  HydroxylPercent) GOType = 2;
	else if ( EpoxyPercent &&  HydroxylPercent) GOType = 3;
	else
	{
		cout << "At least one of the EpoxyPercent and HydroxylPercent Must be Nonzero!!! " << endl << endl;
		return -1;
	}

	switch (GOType)
	{
	case 1: 
		outputFileName = "90X50GO" + eString + "%E.xyz";
		break;
	case 2:
		outputFileName = "90X50GO" + hString + "%H.xyz";
		break;
	case 3:
		outputFileName = "90X50GO" + eString + "%E" + hString + "%H.xyz";
		break;
	}

	bool canIWriteHydrogen = false;
	ifstream input("90X50.xyz", ios::in);
	ofstream output(outputFileName, ios::out);
    ofstream tempOut("tmp.xyz", ios::out);
	ifstream tempIn;



	if (input.is_open())
	{
		std::getline(input, atomCountString);
		std::getline(input, dummy);
	}
	else
	{
		cout << "there is no Input file" << endl;
		return -1;
	}


	int atomCount;
	stringstream(atomCountString) >> atomCount;
	const int initialAtomCount = atomCount;

	Atom* atom = new Atom[atomCount];
    std::string line;

	cout << "Welcome to GRaphene OXide builder (GROX) written by" << endl;
	cout << "Mahdi Tavakol" << endl << endl << endl;
	cout << "Reading Input File ... " << endl;
	for (int i = 0; i < atomCount; i++)
	{
		std::getline(input, line);
		float x, y, z;
		int atomID;
		int atomType = 1; //normal C
		std::string xStr = line.substr(9, 10);
		std::string yStr = line.substr(25, 10);
		std::string zStr = line.substr(41, 10);
		atomID = i;
		stringstream(xStr) >> x;
		stringstream(yStr) >> y;
		stringstream(zStr) >> z;
		// output << line << endl; Since we might want to change the type of some carbon atoms
		// to be incorporated in the united atom model, we prefer to write atoms again.
		atom[i].SetAtom(atomID, 1, x, y, z, true);
	}

	cout << "Reading The Input File Is Completed" << endl << endl;
	cout << "Adding Hydroxyl, Carboxyl And Epoxy Groups ... " << endl;

	input.close();

	/*
	float maxX = 1;
	float minX = 1;
	float maxY = 1;
	float minY = 1;
	float maxZ = 1;
	float minZ = 1;

	for (int i = 0; i < initialAtomCount; i++)
	{
		float x, y, z;
		atom[i].GetCoordinates(x, y, z);
		if (maxX < x)
		{
			maxX = x;
		}
		else if (minX > x)
		{
			minX = x;
		}
		else if (maxY < y)
		{
			maxY = y;
		}
		else if (minY > y)
		{
			minY = y;
		}
		else
		{
		}
	} */

	// cout << minX << endl << maxX << endl << minY << endl << maxY << endl;

	for (int i = 0; i < initialAtomCount; i++)
	{
		// float x, y, z;
		// bool flag;
		// atom[i].GetCoordinates(x, y, z);
		// atom[i].GetFlag(flag);
		int direction = 0;
		/*if (x == maxX ||
			x == minX ||
			y == maxY ||
			y == minY)
		{
			if (x == maxX)
				direction = 1;  //direction = 1;
			if (x == minX)
				direction = 2; //direction = 2;
			if (y == maxY)
				direction = 3;  //direction = 3;
			if (y == minY)
				direction = 4;  //direction = 4;
			int event = 0;
			if ((event = get_event(CarboxylPercent, 0)) == 1)
			{
				int writeCarboxylOutput = 0;
				writeCarboxylOutput = writeCarboxyl(tempOut, direction, i, atom, canIWriteHydrogen);
				atomCount += writeCarboxylOutput;
				if (writeCarboxylOutput && !canIWriteHydrogen)
					atom[i].SetType(2);
			}
			else if (canIWriteHydrogen)
				atomCount += writeHydrogen(tempOut, direction, i, atom);
			else
			{
			}
		}
		else
		{*/
			direction = 5;
			int event = 0;
			event = get_event(EpoxyPercent, HydroxylPercent);
			if (event == 1)
			{
				atomCount += writeEpoxy(tempOut, atom, i, initialAtomCount, GOType, canIWriteHydrogen);
			}
			else if (event == 2)
			{
				atomCount += writeHydroxyl(tempOut, direction, atom[i], GOType, canIWriteHydrogen);
				
			}
			else if (event == 3)
			{
				// cout << event << endl;
			}
			else
			{
				// cout << "Error " << endl;
			}


		//}

		atom[i].PrintAtomsXYZ(tempOut);

	}


	tempOut.close();
	tempIn.open("tmp.xyz");

	// Headers for output files
	output << atomCount << endl;
	output << "GRaphene OXide (GROX) code written by HADI TAVAKOL" << endl;

	while (std::getline(tempIn, line))
		output << line << endl;
	tempIn.close();




	// removing the temp file

	cout << "Deleting Temp Files ... " << endl;
	remove("tmp.xyz");

	summaryOfAtomTypes(GOType, canIWriteHydrogen);

	cout << "All Done, Back To You OS" << endl << endl << endl;

	//system("PAUSE");
	return 0;
	//*********************************************************************//
	//*********************************************************************//
	//*********************************************************************//
	//*********************************************************************//
	//*********************************************************************//
}

int up_or_down()
{
	std::uniform_int_distribution<std::mt19937::result_type> dist(1, 2);
	int r;
	r = dist(rng);
	if (r == 1) return 1; // Up
	if (r == 2) return 2; // Down
	return 0;
}


int get_event(float p1, float p2)
{
	std::uniform_int_distribution<std::mt19937::result_type> dist(1, 100000); // distribution in range [1, 100000]
	p1 *= 1000;
	p2 *= 1000;
	int r;
	r = dist(rng);
	if (r < p1) return 1;
	r -= p1;
	if (r < p2) return 2;
	return 3;
}



int writeHydroxyl(std::ofstream& file, int direction, Atom& atom, int GOType, int CanIWriteHydrogen)
{
	Atom  oxygenAtom;
	Atom  hydrogenAtom;
	float xO, yO, zO, xH, yH, zH;
	bool  flag;
	float HD = 1.48f;
	float MD = 0.96f;
	atom.GetCoordinates(xO, yO, zO);
	atom.GetFlag(flag);
	xH = xO;
	yH = yO;
	zH = zO;

	switch (direction)
	{
	case 1:  /* Right side of the graphene */
	{
		xO += HD;
		xH = xO + MD;
		break;
	}
	case 2:  /* Left side of the graphene */
	{

		xO -= HD;
		xH = xO - MD;
		break;
	}
	case 3:  /* Up side of the graphene   */
	{
		yO += HD;
		yH = yO + MD;
		break;
	}
	case 4:  /* Down */
	{
		yO -= HD;
		yH = yO - MD;
		break;
	}
	case 5:
	{
		if (up_or_down() == 1)
		{
			zO += HD;
			zH = zO + MD*0.951f; // sin(18)
		}
		else
		{
			zO -= HD;
			zH = zO - MD*0.951f; // sin(18)
		}

		xH = xO + MD*0.951f;  //cos(18);
		break;
	}
	}
	if (flag == true)
	{
		if (CanIWriteHydrogen == 1)
		{
			oxygenAtom.SetAtom(0, 2, xO, yO, zO, true);
			hydrogenAtom.SetAtom(0, 3, xH, yH, zH, true);
			oxygenAtom.PrintAtomsXYZ(file);
			hydrogenAtom.PrintAtomsXYZ(file);
			return 2;
		}
		else if (CanIWriteHydrogen == 0)
		{
			if      (GOType == 2) oxygenAtom.SetAtom(0, 3, xO, yO, zO, true);
			else if (GOType == 3) oxygenAtom.SetAtom(0, 4, xO, yO, zO, true);
			else {}
			oxygenAtom.PrintAtomsXYZ(file);
			atom.SetType(2);
			return 1;
		}

	}
	else
	{
		// cout << "sorry !!!" << endl;
		return 0;
	}

	return 0; // I do know that we do not never ever ever get here
}


int writeCarboxyl(std::ofstream& file, int direction, int atomID, Atom atoms[], bool CanIWriteHydrogen)
{
	float xO1, yO1, zO1;
	bool flag;
	Atom oxygenAtom1, oxygenAtom2, hydrogenAtom;
	atoms[atomID].GetCoordinates(xO1, yO1, zO1);
	atoms[atomID].GetFlag(flag);
	float xO2 = xO1;
	float xH = xO1;
	float yO2 = yO1;
	float yH = yO1;
	float zO2 = zO1;
	float zH = zO1;

	switch (direction)
	{
	case 1:  /* Right side of the graphene */
		xO1 += 1.081f;
		yO1 += 0;
		zO1 -= 0.757f;
		xO2 += 0;
		yO2 += 0;
		zO2 += 1.230f;
		xH += 0.96f;
		yH += 0;
		zH += 0;
		break;
	case 2:  /* Left side of the graphene */
		xO1 -= 1.081f;
		yO1 += 0;
		zO1 -= 0.757f;
		xO2 += 0;
		yO2 += 0;
		zO2 += 1.230f;
		xH -= 0.96f;
		yH += 0;
		zH += 0;
	case 3:  /* Up side of the graphene   */
		xO1 += 0;
		yO1 += 1.081f;
		zO1 -= 0.757f;
		xO2 += 0;
		yO2 += 0;
		zO2 += 1.230f;
		xH += 0;
		yH += 0.96f;
		zH += 0;
		break;
	case 4:  /* Down */
		xO1 += 0;
		yO1 -= 1.081f;
		zO1 -= 0.757f;
		xO2 += 0;
		yO2 += 0;
		zO2 += 1.230f;
		xH += 0;
		yH -= 0.96f;
		zH += 0;
		break;
	case 5:
		/* There should be no case 5  for this function */
		break;
	}

	if (flag == true)
	{
		oxygenAtom2.SetAtom(0, 3, xO2, yO2, zO2, true);
		oxygenAtom2.PrintAtomsXYZ(file);

		if (CanIWriteHydrogen)
		{
			oxygenAtom1.SetAtom(0, 3, xO1, yO1, zO1, true);  // normal O
			oxygenAtom1.PrintAtomsXYZ(file);
			hydrogenAtom.SetAtom(0, 5, xH, yH, zH, true);
			hydrogenAtom.PrintAtomsXYZ(file);
			return 3;
		}

		else
		{
			oxygenAtom1.SetAtom(0, 4, xO1, yO1, zO1, true);  // United Atom O
			oxygenAtom1.PrintAtomsXYZ(file);
			return 2;
		}


		// cout << "Carboxyling !!!" << endl;
		atoms[atomID].SetFlag(false);
		return 3;
	}

	else
	{
		//  cout << "Impossible" << endl;
		return 0;
	}
}

int writeHydrogen(std::ofstream& file, int direction, int atomID, Atom atoms[])
{
	float xH;
	float yH;
	float zH;
	bool flag;
	atoms[atomID].GetCoordinates(xH, yH, zH);
	atoms[atomID].GetFlag(flag);
	Atom hydrogenAtom;


	switch (direction)
	{
	case 1:  /* Right side of the graphene */
		xH += 0.96f;
		// cout << "Hey rodio" << endl; 
		break;
	case 2:  /* Left side of the graphene */
		xH -= 0.96f;
		break;
	case 3:  /* Up side of the graphene */
		yH += 0.96f;
		break;
	case 4:  /* Down */
		yH -= 0.96f;
		break;
	case 5:
		/* There should be no case 5  for this function */
		break;
	}

	if (flag == true)
	{
		hydrogenAtom.SetAtom(0, 5, xH, yH, zH, true);
		hydrogenAtom.PrintAtomsXYZ(file);
		return 1;
	}

	else
	{
		//  cout << "Impossible" << endl;
		return 0;
	}
}


float distance(Atom atom1, Atom atom2)
{
	float x1, y1, z1, x2, y2, z2;
	atom1.GetCoordinates(x1, y1, z1);
	atom2.GetCoordinates(x2, y2, z2);
	float dis = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2) + pow(z1 - z2, 2));
	return dis;
}


int selectEpoxy(Atom atoms[], int atomID, int atomCount)
{
	int neighbor[3] = { 0, 0, 0 };
	int j = 0;
	for (int i = 0; i < atomCount; i++)
	{
		float dist = distance(atoms[atomID], atoms[i]);
		if (dist <= 1.52 && dist >= 1.32)
		{
			neighbor[j] = i;
			j++;
		}
		if (j == 3)
		{
			break;
		}
	}

	int k = get_event(33, 33);
	return neighbor[k - 1];
}



int writeEpoxy(std::ofstream& file, Atom atom[], int i, int initialAtomCount, 
	int GOType, int canIWriteHydrogen)
{
	int j = selectEpoxy(atom, i, initialAtomCount);
	float x1, y1, z1, x2, y2, z2;
	bool flag1, flag2;
	atom[i].GetCoordinates(x1, y1, z1);
	atom[j].GetCoordinates(x2, y2, z2);
	atom[i].GetFlag(flag1);
	atom[j].GetFlag(flag2);
	float xO = (x1 + x2) / 2;
	float yO = (y1 + y2) / 2;
	float zO;
	int upOrDown = up_or_down();

	if (upOrDown == 1)  zO = 1.276f;
	else if (upOrDown == 2)  zO = -1.276f;
	else {}

	/* We suppose that the C-C bond length is the same as the length for graphene
	and also the C-O bond length is 1.49 A. Another assumption is that the C-C-O is perpendicular
	to the graphene rings */
	if (flag1 == true && flag2 == true)
	{
		Atom oxygenAtom;
		if (!canIWriteHydrogen)
		{
			if      (GOType == 1) oxygenAtom.SetAtom(0, 2, xO, yO, zO, true);
			else if (GOType == 3) oxygenAtom.SetAtom(0, 3, xO, yO, zO, true);
			else {}
		}
		else oxygenAtom.SetAtom(0, 2, xO, yO, zO, true);

		oxygenAtom.PrintAtomsXYZ(file);
		atom[i].SetFlag(false);
		atom[j].SetFlag(false);
		return 1;
	}
	else
	{
		// cout << "Oops! the valence layer of carbons is full !!!" << endl;
		return 0;
	}
}

void summaryOfAtomTypes(int GOType, int canIWriteHydrogen)
{
	cout << endl << endl << endl;
	switch (GOType)
	{
	case 1:
		cout << "AtomType                         TypeNumber" << endl;
		cout << "C                                         1" << endl;
		cout << "O                                         2" << endl;
		cout << "-------------------------------------------" << endl;
		break;
	case 2:
		if (canIWriteHydrogen)
		{
			cout << "AtomType                         TypeNumber" << endl;
			cout << "C                                         1" << endl;
			cout << "O                                         2" << endl;
			cout << "H                                         3" << endl;
			cout << "-------------------------------------------" << endl;
		}
		else
		{
			cout << "AtomType                         TypeNumber" << endl;
			cout << "Normal C                                  1" << endl;
			cout << "C Connected to UA Hydroxyl                2" << endl;
			cout << "UA O                                      3" << endl;
			cout << "-------------------------------------------" << endl;
		}
		break;
	case 3:
		cout << "AtomType                         TypeNumber" << endl;
		cout << "Normal C                                  1" << endl;
		cout << "C Connected to UA Hydroxyl                2" << endl;
		cout << "Normal O                                  3" << endl;
		cout << "UA O                                      4" << endl;
		cout << "H                                         5" << endl;
		cout << "-------------------------------------------" << endl;
		break;

	}
}
