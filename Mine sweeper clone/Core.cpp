#include "Core.h"

Core::Core( int n_m )
:
n_mines( n_m )
{
	int size = GetArraySize();
	for( int i = 0; i < size; i++ )
	{
		WriteToArray( values,i,0 );
		WriteToArray( bools,i,0 );

		WriteToArray( values,cursorX,cursorX,GetFromArray( values,cursorX,cursorY ) + 10 ); //cursor initialization
	}
}

Core::~Core()
{
	delete []values;
	delete []bools;
}

void Core::GenerateField()
{
	std::mt19937 rng;
	rng.seed( std::random_device()() );


	std::uniform_int_distribution< int > rand_width( 0,width - 1 );
	std::uniform_int_distribution< int > rand_height( 0,height - 1 );

	int counter = 0;

	while( counter < n_mines )
	{
		int w = rand_width( rng );
		int h = rand_height( rng );

		if( GetFromArray( values,w,h ) != 9 )
		{
			WriteToArray( values,w,h,9 );
			counter++;
		}
	}

	for( int y = 0; y < height; y++ )
	{
		for( int x = 0; x < width; x++ )
		{
			if( GetFromArray( values,x,y ) != 9 )
			{
				WriteToArray( values,x,y,MinesAround( x,y ) );
			}
		}
	}

}

const int Core::MinesAround( int row,int column )
{
	int sum = 0;

	if( ( row - 1  >= 0 && column - 1 >= 0 ) && GetFromArray( values,row - 1,column - 1 ) == 9 ) sum++; 
	if( ( row - 1  >= 0 ) && GetFromArray( values,row - 1,column ) == 9 ) sum++;
	if( ( row - 1  >= 0 && column + 1 < height ) && GetFromArray( values,row - 1,column + 1 ) == 9 ) sum++;

	if( ( column - 1 >= 0 ) && GetFromArray( values,row,column - 1 ) == 9 ) sum++;
	if( ( column + 1 < height ) && GetFromArray( values,row,column + 1 ) == 9 ) sum++;

	if( ( row + 1  < width && column - 1 >= 0 ) && GetFromArray( values,row + 1,column - 1 ) == 9 ) sum++;
	if( ( row + 1  < width ) && GetFromArray( values,row + 1,column ) == 9 ) sum++;
	if( ( row + 1  < width && column + 1 < height ) && GetFromArray( values,row + 1,column + 1 ) == 9 ) sum++;

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

	for( int a = 0; a < 2 * width + 3; a++ )
	{
		std::cout << "-";
	}
	std::cout << std::endl;

	for( int y = 0; y < height; y++ )
	{
		std::cout << "|";
		for( int x = 0; x < width; x++ )
		{
			if( GetFromArray( bools,x,y ) == 0 ) //uncovered field
			{
				if( ( x == cursorX && y == cursorY ) || ( x - 1 == cursorX && y == cursorY ) )
				{
					std::cout << "|X";
				}
				else
				{
					std::cout << " X";
				}
			}
			else
			{
				if( GetFromArray( values,x,y ) == 0 )
				{
					std::cout << "  ";
				}
				else
				{
					std::cout << " " << GetFromArray( values,x,y );
				}
			}
		}
		if( cursorX == width - 1 && y == cursorY )
		{
			std::cout << "||";
		}
		else
		{
			std::cout << " |";
		}
		std::cout << std::endl;
	}

	for( int a = 0; a < 2 * width + 3; a++ )
	{
		std::cout << "-";
	}
	std::cout << std::endl;
}

void Core::HandleCursor( char dir )
{
	if( cursorX < 1 ) cursorX = 1;
	if( cursorX >= width -1 ) cursorX = width - 2;
	if( cursorY < 1 ) cursorY = 1;
	if( cursorY >= height - 1 ) cursorY = height - 2;
	

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
}

void Core::WriteToArray( int *arr,int index,int value )
{
	arr[ index ] = value;
}

void Core::WriteToArray( int *arr,int row,int column,int value )
{
	arr[ width * column + row ] = value;
}

const int Core::GetFromArray( int *arr,int index )
{
	return arr[ index ];
}

const int Core::GetFromArray( int *arr,int row,int column )
{
	return arr[ width * column + row ];
}


inline const int Core::GetArraySize()
{
	return width * height; 
}