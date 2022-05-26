#include "Atoms.h"
#include <string>
#include <fstream>
#include <iomanip>
using namespace std;

// class Atom definition
Atom::Atom(int atomIDVal, int atomTypeVal, float xVal, float yVal, float zVal, bool flagValue )
{
	atomID         = atomIDVal;
	atomType       = atomTypeVal;
	x              = xVal;
	y              = yVal;
	z              = zVal;
	flag           = flagValue;
}

Atom::Atom( const Atom& myAtom ):
	atomID( myAtom.atomID), atomType( myAtom.atomType),
	x( myAtom.x ), y( myAtom.y ), z( myAtom.z ),
	flag( myAtom.flag ), atomChar( myAtom.atomChar )
{}

void Atom::SetAtom(int atomIDVal, int atomTypeVal, float xVal, float yVal, float zVal, bool flagValue )
{
	atomID         = atomIDVal;	
	atomType       = atomTypeVal;
	x              = xVal;
	y              = yVal;
	z              = zVal;
	flag           = flagValue;
}


void Atom::PrintAtomsXYZ(ofstream& file)
{
	
	file << " ";
	file.width(8);
	file << fixed << left << atomType ;
	file.width(10)                                           ;
	file << fixed << setprecision(6) << right << x           ;
	file << "      "                                         ;
	file.width(10)                                           ;
	file << fixed << setprecision(6) << right << y           ;
	file << "      "                                         ;
	file.width(10)                                           ;
	file << fixed << setprecision(6) << right << z           ;
	file << endl                                             ;
}

void Atom::PrintAtomsXYZ2(ofstream& file)
{
	this->SetAtomChar();
	file << " ";
	file.width(8);
	file << fixed << left << atomChar;
	file.width(10);
	file << fixed << setprecision(6) << right << x;
	file << "      ";
	file.width(10);
	file << fixed << setprecision(6) << right << y;
	file << "      ";
	file.width(10);
	file << fixed << setprecision(6) << right << z;
	file << endl;
}


int Atom::GetAtomID()
{
	return atomID;
}


void  Atom::GetCoordinates(float &xVal, float&yVal, float&zVal )
{
	xVal = x;
	yVal = y;
	zVal = z;
}

void Atom::GetFlag(bool &flagStatues)
{
	flagStatues = flag;
}

void Atom::SetFlag(const bool &newflag)
{
	flag = newflag;
}

void Atom::SetType(const int &newtype)
{
	atomType = newtype;
}

void Atom::SetAtomChar()
{
	switch (atomType)
	{
	case 1:
	case 2:
	{
		atomChar = 'C';
		break;
	}
	case 3:
	case 4:
	{
		atomChar = 'O';
		break;
	}
	case 5:
	{
		atomChar = 'H';
		break;
	}
	default:
		atomChar = 'C';
	}
}
