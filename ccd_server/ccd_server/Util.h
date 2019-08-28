#pragma once
#include <string>
#include<iostream>

using namespace std;

class Util
{
public:
	Util() {

	}

	string getString() {
		printHello();
	}


	~Util() {

	}

private:
	void printHello() {
		cout << "Hello wold!";
	}

};
