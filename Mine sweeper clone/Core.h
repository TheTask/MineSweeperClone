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
		void HandleCursor( char dir );

		inline void WriteToArray( int *arr,int index,int value );
		inline void WriteToArray( int *arr,int row,int column,int value );

		inline const int GetFromArray( int *arr,int index );
		inline const int GetFromArray( int *arr,int row,int column );

		inline const int GetCursorX() { return cursorX; }
		inline const int GetCursorY() { return cursorY; }

		inline const int GetArraySize();

	private:
		const int width = 40;
		const int height = 25;

		int cursorX = width / 2;
		int cursorY = height / 2;

		int *values = new int[ width * height ];
		int *bools = new int[ width * height ];
		int n_mines;

		char dir;


};