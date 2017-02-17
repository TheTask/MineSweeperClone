#include "Core.h"
#include <conio.h>



int main()
{
	int nm;
	char direction = 'x';

	//std::cin >> nm;
	nm = 50;

	Core core( nm );
	core.GenerateField();


	core.DisplayField();

	for( ;; )
	{
		while( _kbhit() )
		{
			direction = _getch();
			core.HandleCursor( direction );
			core.DisplayField();
		}
	}
}