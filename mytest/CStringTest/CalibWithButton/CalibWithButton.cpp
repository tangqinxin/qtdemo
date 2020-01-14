#include <iostream>
#include "CalibCam.h"
#include "MatIO.h"
#include "myfun.h"
#include <string>

using namespace std;

int main() {

	string FileName = "calibdata.txt";
	int width = 11;
	int height = 8;
	float squaresize = 3;
	CalibWithButton(FileName,width,height,squaresize);

	return 0;
}