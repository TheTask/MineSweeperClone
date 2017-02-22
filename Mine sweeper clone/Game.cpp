#include "Core.h"
#include <conio.h>



int main()
{
	int nm;
	char key_press = 'x';

	std::cout << "Minesweeper clone created by Marek Borik." << std::endl;
	std::cout << std::endl;
	std::cout << "Navigate by using WASD, flag / unflag by pressing the F key and reveal a square by pressing the Q key." << std::endl;
	std::cout << "Start by typing the number of mines you want to start with: " << std::endl;


	std::cin >> nm;

	while( nm > 750 || nm < 1 )
	{
		std::cout << "The number you have entered is invalid or too big, try again: " << std::endl;
		std::cin >> nm;
	}

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
			else if( key_press == 'q' ) //uncover a square
			{
				if( core.GetMineState( core.GetCursorX(),core.GetCursorY() ) == 1 ) //mine uncovered
				{
					core.GameLost();
					return 0;
				}
				else
				{
					core.UncoverSquare( core.GetCursorX(),core.GetCursorY() );
				}
			}
			else if( key_press == 'f' ) //flag square
			{
				if( core.GetFlagState( core.GetCursorX(),core.GetCursorY() ) == 0 ) //unflagged square
				{
					core.FlagSquare( core.GetCursorX(),core.GetCursorY() );
				}
				else //already flagged square
				{
					core.UnFlagSquare( core.GetCursorX(),core.GetCursorY() );
				}
			}
			core.DisplayField();
			
			if( core.GameWon() )
			{
				std::cout << "Congratulations! You have won the game!!!" << std::endl;
				return 0;
			}
		}
	}
}