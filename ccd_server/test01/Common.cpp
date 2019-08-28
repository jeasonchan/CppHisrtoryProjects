#include "pch.h"
#include "Common.h"
#include <iostream>

using namespace std;

string Common::palnet = "";

Common::Common()
{
}

Common::Common(int age, string name)
{
	this->age = age;
	this->name = name;

}


Common::~Common()
{
}

void Common::printThis()
{
	try {
		if (this->age == 0 || this->name.length()==0) {
			throw new exception("not inilitize.");
		}
	}
	catch (exception* e) {
		cout <<e->what();
	}
	cout << this->age << "\n" << this->name;
}
