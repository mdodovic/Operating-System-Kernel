#include "System.h"

extern int userMain(int argc, char* argv[]);
//extern int userMainA(int argc, char* argv[]);

int main(int argc, char* argv[]) 
{
	System::inic();
	int mainValue = userMain(argc, argv);
	System::restore();
	return mainValue;
}
