#pragma once
#include <iostream>
#include <random>
#include <windows.h>


class Core
{
	public:
		Core( short n_m );
		~Core();

		void GenerateField();
		const short MinesAround( short &row,short &column );

		void WipeConsole();
		const void DisplayField();

		template< typename T >
		const void DrawWithCursor( T to_draw,short color,short &x,short &y );

		void UncoverSquare( short x,short y );
		void FlagSquare( short x,short y );
		void UnFlagSquare( short x,short y );

		void HandleCursor( char &dir );


		inline void SetValue( short &row,short &column,short value );

		inline const short GetValue( short &row,short &column );
		inline const bool GetMineState( short row,short column );
		inline const bool GetCoverState( short &row,short &column );
		inline const bool GetFlagState( short row,short column );

		inline const short GetCursorX() { return cursorX; }
		inline const short GetCursorY() { return cursorY; }

		inline const short GetArraySize();

		const bool GameWon();
		const void GameLost();

	private:
		const short width = 40;
		const short height = 25;

		short cursorX = width / 2;
		short cursorY = height / 2;

		struct Square
		{
			short x;
			short y;
			short value;
			bool mine;
			bool cover;
			bool flag;
		};

		Square *squares = new Square[ width * height ];

		short n_mines;
		char dir;
		const short cursor_color = 12; //red
};