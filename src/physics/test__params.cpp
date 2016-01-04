#include "Params.h"


void test__params() {
	Params<position_type> p_seperated  (2,2, Params<position_type>::Interleaved);
	Params<position_type> p_interleaved(2,2, Params<position_type>::Seperated  );

	Params<position_type>* ptr__params[2];

	ptr__params[0] 	= &p_seperated;
	ptr__params[1] 	= &p_interleaved;

	for (int i = 0; i < 2; ++i)
	{
		ptr__params[i]->setVal(X, 0, -1);
		ptr__params[i]->setVal(Y, 0, -1);
		
		ptr__params[i]->setVal(X, 1,  1);
		ptr__params[i]->setVal(Y, 1,  1);
	}

	for (int i = 0; i < 2; ++i)
	{
		ptr__params[i]->printAllElements();
	}
}

#ifdef CLASS_TEST
int main(int argc, char const *argv[])
{
	test__params();
	return 0;
}
#endif