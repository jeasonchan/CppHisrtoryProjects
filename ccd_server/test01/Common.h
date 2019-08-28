#pragma once
#include<string>

using namespace std;


class Common
{
public:
	Common();
	Common(int age,string name);
	~Common();
	void printThis();
	static string palnet;

private:
	int age=0;
	string name="";

};

