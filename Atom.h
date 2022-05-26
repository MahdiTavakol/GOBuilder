#ifndef ATOM_H
#define ATOM_H
#include <fstream>
using namespace std;

// class Atom definition
class Atom
{
	public:
	    Atom(int = 0, int = 0, float = 0.0f , float = 0.0f , float =  0.0f, bool = false ); // constructor
		Atom(const Atom&); // copy constructor
	    void SetAtom(int , int , float , float , float , bool );
		void PrintAtomsXYZ(ofstream& );
		void PrintAtomsXYZ2(ofstream&); //just for debugging purposes
        int  GetAtomID();
        void GetCoordinates(float&, float&, float& );
		void GetFlag(bool& );
		void SetFlag(const bool&);
		void SetType(const int&);
		void SetAtomChar(); //just for debugging purposes

	private:
		
		int atomID;
		int atomType;
		float x;
		float y;
		float z;
		bool flag; // To check the number of covalent bonds
		char atomChar; // just for debugginf purposes
}; // end class Atom

#endif
