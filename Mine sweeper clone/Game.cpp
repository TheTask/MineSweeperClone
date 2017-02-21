#include "Core.h"
#include <conio.h>



int main()
{
	int nm;
	char key_press = 'x';

	//std::cin >> nm;
	nm = 20;

	Core core( nm );
	core.GenerateField();


	core.DisplayField();

	for( ;; )
	{
		while( _kbhit() )
		{
			key_press = _getch();
			if( key_press == 'w' || key_press == 'a' || key_press == 's' || key_press == 'd' )
			{
				core.HandleCursor( key_press );
			}
			if( key_press == 'o' )
			{
				core.UncoverSquare( core.GetCursorX(),core.GetCursorY() );
			}
			core.DisplayField();
		}
	}
}