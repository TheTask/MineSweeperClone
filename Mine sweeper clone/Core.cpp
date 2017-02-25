#include "Core.h"

Core::Core( short n_m )
:
n_mines( n_m )
{
	int size = GetArraySize();
	for( short i = 0; i < size; i++ )
	{
		squares[ i ].x = i % width;
		squares[ i ].y = i / width;
		squares[ i ].value = 0;
		squares[ i ].mine = 0;
		squares[ i ].cover = 1;
		squares[ i ].flag = 0;
	}
}

Core::~Core()
{
	delete []squares;
}

void Core::GenerateField()
{
	std::mt19937 rng;
	rng.seed( std::random_device()() );


	std::uniform_int_distribution< short > rand_width( 0,width - 1 );
	std::uniform_int_distribution< short > rand_height( 0,height - 1 );

	short counter = 0;

	while( counter < n_mines )
	{
		short x = rand_width( rng );
		short y = rand_height( rng );

		if( GetMineState( x,y ) == 0 )
		{
			squares[ width * y + x ].mine = 1;
			counter++;
		}
	}

	for( short y = 0; y < height; y++ )
	{
		for( short x = 0; x < width; x++ )
		{
			if( GetMineState( x,y ) == 0 )
			{
				SetValue( x,y,MinesAround( x,y ) );
			}
		}
	}
}

const short Core::MinesAround( short &row,short &column )
{
	short sum = 0;

	if( ( row - 1  >= 0 && column - 1 >= 0 ) && GetMineState( row - 1,column - 1 ) == 1 ) sum++; 
	if( ( row - 1  >= 0 ) && GetMineState( row - 1,column ) == 1 ) sum++;
	if( ( row - 1  >= 0 && column + 1 < height ) && GetMineState( row - 1,column + 1 ) == 1 ) sum++;

	if( ( column - 1 >= 0 ) && GetMineState( row,column - 1 ) == 1 ) sum++;
	if( ( column + 1 < height ) && GetMineState( row,column + 1 ) == 1 ) sum++;

	if( ( row + 1  < width && column - 1 >= 0 ) && GetMineState( row + 1,column - 1 ) == 1 ) sum++;
	if( ( row + 1  < width ) && GetMineState( row + 1,column ) == 1 ) sum++;
	if( ( row + 1  < width && column + 1 < height ) && GetMineState( row + 1,column + 1 ) == 1 ) sum++;

	return sum;
}

void Core::WipeConsole()
{
	HANDLE hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
	COORD coord = { 0,0 };
	DWORD count;
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if( GetConsoleScreenBufferInfo( hStdOut,&csbi ) )
	{
		FillConsoleOutputCharacter( hStdOut,(TCHAR)32,csbi.dwSize.X * csbi.dwSize.Y,coord,&count );
		FillConsoleOutputAttribute( hStdOut,csbi.wAttributes,csbi.dwSize.X - 1* csbi.dwSize.Y - 1,coord,&count );
		SetConsoleCursorPosition( hStdOut,coord );
	}
	return;
}

const void Core::DisplayField()
{
	WipeConsole();
    HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE ); 
	SetConsoleTextAttribute( hConsole,14 ); //yellow

	for( short a = 0; a < 2 * width + 3; a++ )
	{
		std::cout << "-";
	}
	std::cout << std::endl;

	for( short y = 0; y < height; y++ )
	{
		SetConsoleTextAttribute( hConsole,14 ); //white
		std::cout << "|";
		for( short x = 0; x < width; x++ )
		{
			bool isCursorThere = ( x == cursorX && y == cursorY ) || ( x - 1 == cursorX && y == cursorY );

			if( GetFlagState( x,y ) == 1 ) //flagged field
			{
				DrawWithCursor( "F",10,x,y ); //green
			}
			else if( GetCoverState( x,y ) == 1 ) //covered field
			{
				DrawWithCursor( "X",15,x,y ); //white
			}
			else if( GetCoverState( x,y ) == 0 && GetMineState( x,y ) == 1 )
				DrawWithCursor( "M",13,x,y ); //purple
			else //uncovered field
			{
				if( GetValue( x,y ) == 0 )
				{
					DrawWithCursor( " ",15,x,y ); //doesn't matter
				}
				else
				{
					DrawWithCursor( GetValue( x,y ),11,x,y ); //lt blue
				}
			}
		}
		if( cursorX == width - 1 && y == cursorY ) //right border if cursor is touching it
		{
			SetConsoleTextAttribute( hConsole,cursor_color ); //red
			std::cout << "|";
			SetConsoleTextAttribute( hConsole,14 ); //yellow
			std::cout << "|";
		}
		else //right border if cursor is not touching it
		{
			SetConsoleTextAttribute( hConsole,14 ); //yellow
			std::cout << " |";
		}

		std::cout << std::endl;
	}

	for( short a = 0; a < 2 * width + 3; a++ ) //bottom line
	{
		std::cout << "-";
	}
	std::cout << std::endl;
}

template <typename T>
const void Core::DrawWithCursor( T to_draw,short color,short &x,short &y )
{
	bool isCursorThere = ( x == cursorX && y == cursorY ) || ( x - 1 == cursorX && y == cursorY );

	HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE ); 
	
	if( isCursorThere ) 
	{
		SetConsoleTextAttribute( hConsole,cursor_color ); //red
		std::cout << "|";
		SetConsoleTextAttribute( hConsole,color ); //chosen color
		std::cout << to_draw;
	}
	else
	{
		SetConsoleTextAttribute( hConsole,color ); //chosen color
		std::cout << " " << to_draw;
	}
}

void Core::UncoverSquare( short x,short y )
{
	if( GetCoverState( x,y ) == 1 && x >= 0 && y >= 0 && x < width && y < height )
	{
		if( GetValue( x,y ) == 0 )
		{
			squares[ width * y + x ].cover = 0;
			UncoverSquare( x - 1,y     );
			UncoverSquare( x + 1,y     );
			UncoverSquare( x    ,y - 1 );
			UncoverSquare( x    ,y + 1 );
		}
		else
		{
			squares[ width * y + x ].cover = 0;
		}
	}
}

void Core::FlagSquare( short x,short y )
{
	squares[ width * y + x ].flag = 1;
}

void Core::UnFlagSquare( short x,short y )
{
	squares[ width * y + x ].flag = 0;
}

void Core::HandleCursor( char &dir )
{
	if( dir == 'w' )
	{
		cursorY--;
	}
	else if( dir == 's' )
	{
		cursorY++;
	}
	else if( dir == 'a' )
	{
		cursorX--;
	}
	else if( dir == 'd' )
	{
		cursorX++;
	}

	if( cursorX < 0 ) cursorX = 0;
	if( cursorX >= width ) cursorX = width - 1;
	if( cursorY < 0 ) cursorY = 0;
	if( cursorY >= height ) cursorY = height - 1;
}

inline void Core::SetValue( short &row,short &column,short value )
{
	squares[ width * column + row ].value = value;
}

const short Core::GetValue( short &row,short &column )
{
	return squares[ width * column + row ].value;
}

const bool Core::GetMineState( short row,short column )
{
	return squares[ width * column + row ].mine;
}

const bool Core::GetCoverState( short &row,short &column )
{
	return squares[ width * column + row ].cover;
}

const bool Core::GetFlagState( short row,short column )
{
	return squares[ width * column + row ].flag;
}

const short Core::GetArraySize()
{
	return width * height; 
}

const bool Core::GameWon()
{
	short counter = 0;
	for( short i = 0; i < GetArraySize(); i++ )
	{
		if( squares[ i ].cover == 0 )
		{
			counter++;
		}
	}
	if( counter + n_mines == GetArraySize() ) return 1;
	else return 0;

}

const void Core::GameLost()
{
	for( short i = 0; i < GetArraySize(); i++ )
	{
		squares[ i ].cover = 0;
	}
	DisplayField();

	HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE ); 
	SetConsoleTextAttribute( hConsole,12 ); //red
	std::cout << "You have lost the game!" << std::endl;
	SetConsoleTextAttribute( hConsole,15 ); //white
}