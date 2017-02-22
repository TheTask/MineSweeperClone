#pragma once
#include <iostream>
#include <random>
#include <windows.h>


class Core
{
	public:
		Core( int n_m );
		~Core();

		void GenerateField();
		const int MinesAround( int row,int column );

		void WipeConsole();
		const void DisplayField();

		void UncoverSquare( int x,int y );
		void FlagSquare( int x,int y );
		void UnFlagSquare( int x,int y );

		void HandleCursor( char dir );


		inline void SetValue( int row,int column,int value );


		inline const int GetValue( int row,int column );
		inline const bool GetMineState( int row,int column );
		inline const bool GetCoverState( int row,int column );
		inline const bool GetFlagState( int row,int column );

		inline const int GetCursorX() { return cursorX; }
		inline const int GetCursorY() { return cursorY; }

		inline const int GetArraySize();

		const bool GameWon();
		const void GameLost();

	private:
		const int width = 40;
		const int height = 25;

		int cursorX = width / 2;
		int cursorY = height / 2;

		struct Square
		{
			int x;
			int y;
			int value;
			bool mine;
			bool cover;
			bool flag;
		};

		Square *squares = new Square[ width * height ];

		int n_mines;
		char dir;
};