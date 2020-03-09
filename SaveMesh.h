#pragma once

#include <string>

using namespace std;

class SaveMesh
{
private:
	string m_sLogFname;

public:
	void init();
	void logMsg(const char* msg);
	bool saveActiveMesh();
};
