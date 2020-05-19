#include <iostream>
#include <ctime>
#include <cstdlib>
#include <stdlib.h>
#include <time.h>
//////под конец нужно поменять все названия по стилю snake_case
int RndNum(int min, int max)
{
    static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0); 
    
    return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}
int main()
{
	setlocale(0, "");

	using namespace std;

	int large = 50;
	string level[60][60];

	for (int i = 0; i < large; i++)
	{
		for (int j = 0; j < large; j++)
		{
			level[i][j] = "o";
		}
	}
	int ppp = 0;


	srand(time(NULL));
	int pointX = RndNum(1, large - 9);
	int pointy = RndNum(1, large - 9);
	int Vertical;
	int Horizontal;

	int ZamV, ZPY, ZPX, ZamH;
	int IwhileI = 0;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*                                   З Д Е С Ь   Н А Ч И Н А Е Т С Я   Ц И К Л                                                               */

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (IwhileI = 0; IwhileI <= 4; IwhileI++) 
	{
		int  ran;
		int ran1;

		do
		{
			srand(time(NULL));
			ran = RndNum(5, 8);
			ran1 = RndNum(5, 8);

			ZPY = pointy + ran;
			ZPX = pointX + ran1;

			Vertical = ZamV;
			Horizontal = ZamH;
		} while (Vertical > large && Horizontal > large);

		//поменяла названия, для того, чтобы подставлять туда координаты [i][j] от коридора 
		int one_pointy, one_pointx, two_vertical, two_horizontal;
		one_pointy = pointy;
		one_pointx = pointX;
		two_vertical = Vertical;
		two_horizontal = Horizontal;

		//создание комнаты
		for (int i = one_pointy; i < two_vertical; i++)
		{
			for (int j = one_pointx; j < two_horizontal; j++)
			{
				level[i][j] = "Ш";
			}


			for (int i = one_pointy + 1; i < two_vertical - 1; i++)
			{
				for (int j = one_pointx + 1; j < two_horizontal - 1; j++)
				{
					level[i][j] = " ";
				}
			}
		}


		//счетчик пространства за пределами комнаты
		int k = 0, l = 0, k2 = 0, l2 = 0;
		for (int i = 0; i < pointy; i++)
		{
			if (level[i][1] == "o") {
				k++;// кол-во символов c верху до середины
			}
		}
		for (int i = (Vertical); i < large; i++)
		{
			if (level[i][1] == "o") {
				l++;// кол-во символов с середины до низа
			}
		}
		for (int j = 0; j < pointX; j++)
		{
			if (level[1][j] == "o") {
				k2++;// кол-во символов cлева до середины
			}
		}
		for (int j = (Horizontal); j < large; j++)
		{
			if (level[1][j] == "o") {
				l2++;// кол-во символов с середины до правой стороны
			}
		}


		//Рандомный выбор выхода из комнаты сверху по вертикали
		srand(time(0));
		int Vertica = Vertical - 2;
		int ran3 = RndNum(pointy + 1, (Vertica));

		//Рандомный выбор выхода из комнаты слева по горизонтали
		srand(time(0));
		int Horizonta = Horizontal - 2;
		int ran4 = RndNum(pointX + 1, (Horizonta));

		//рандомный выбор выхода из комнаты справа по вертикали
		int schet = 0;
		int leng;
		leng = RndNum(2, 3);


		int rand1, rand2, rand3;
		rand3 = RndNum(1, large);//концовка
		rand2 = RndNum(rand3, large);
		rand1 = RndNum(1, rand3);// от начала до концовки 

		int yyy, yyy1;

		///Генерация коридоров, определение его направления и рандом на размер
		if (k >= k2 && k >= l && k >= l2)
		{

			level[pointy][ran4] = " ";

			int y = pointy;
			int r2 = ran4;
			// Vertical - константа. От x и у до х и Vertical
			for (int i = 0; schet <= leng; i++)
			{
				for (int j = r2; schet <= leng; r2 = y - 1)
				{

					level[y][j] = " ";
					level[y][j + 1] = "Ш";
					level[y][j - 1] = "Ш";
					schet++;
					y = y - 1;

					
				}


				
			}
			
		}
		else if (k2 >= k && k2 >= l && k2 >= l2)
		{//Horizontal - константа. От х и у до Horizontal и у
			int point = pointX;
			int r = ran3;
			level[ran3][pointX] = " ";
			for (int i = r; schet <= leng; r = point)
			{
				for (int j = 0; schet <= leng; j++)
				{

					level[i][point] = " ";
					level[i - 1][point] = "Ш";
					level[i + 1][point] = "Ш";

					schet++;
					point = point -1;
					
				}
			}

			

		}
		else if (l >= k && l >= l2 && l >= k2)
		{
			// Vertical - константа. От рандом х и Vertical до  рандом х и у
			int V = Vertical - 1;
			int r1 = ran4;
			level[V][ran4] = " ";
			for (int i = 0; schet <= leng; i++)
			{
				for (int j = r1; schet <= leng; r1 = V)
				{

					level[V][j] = " ";
					level[V][j + 1] = "Ш";
					level[V][j - 1] = "Ш";
					schet++;
					V = V + 1;
					
				}
			}

			
		}
		else if (l2 >= k && l2 >= l && l2 >= k2)
		{
			int r3 = ran3;
			int H = Horizontal - 1;
			// Horizontal - константа. От  Horizontal и у  до  х и y 
			level[r3][H] = " ";
			for (int i = r3; schet <= leng; r3 = H)
			{
				for (int j = 0; schet <= leng; j++)
				{

					level[i][H] = " ";
					level[i - 1][H] = "Ш";
					level[i + 1][H] = "Ш";
					schet++;
					H = H + 1;
					

					
				}
			}
			
		}

		for (int i = 0; i < large; i++)
		{
			for (int j = 0; j < large; j++)
			{
				cout << (level[i][j]);
			}
			cout << endl;
		}
		cout << endl;
	}
		











		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		/*                                   З Д Е С Ь   З А К А Н Ч И В А Е Т С Я   Ц И К Л                                                          */

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		/*cout << endl;
		cout << "H= " << Horizontal;

		cout << endl;
		cout << "V= " << Vertical;

		cout << endl;
		cout << "X=" << pointX;

		cout << endl;
		cout << "Y=" << pointy;

		cout << endl;
		cout << "сверху до середины " << k;

		cout << endl;
		cout << "слева до середины " << k2;


		cout << endl;
		cout << "с середины до низу " << l;


		cout << endl;
		cout << "с середины до права " << l2;

		cout << endl;
		cout << "ran3 " << ran3;
		cout << endl;
		cout << "ran4 " << ran4;
		cout << endl;
		cout << "ran " << ran;
		cout << endl;
		cout << "ran1 " << ran1;
		cout << endl;
		cout << "leng " << leng;
		cout << endl;
		cout << "rand1 " << rand1;
		cout << endl;
		cout << "rand2 " << rand2;
		cout << endl;
		cout << "rand3 " << rand3;*/


		//cout << "ууу-1 " << yyy-1;
		  // Проблема №1. Я делала комнату ИЗ коридора (брала за основу конечную стену коридора и пыталась от нее построить комнату снизу вверх. 
			  //В общем-то это сделать можно, но придется переписывать метод). 
			  // Мне нужно сделать так, чтобы массив строился так же сверху вниз (это заодно избавит меня от проблемы моно/стенного коридора) и что все константы,
			  // которые берутся на обрыве коридора, являются остановочным "пунктом" новой комнаты. (от рандом значений до определенной строки/столбца 
			  //на длинне/высоте которой закончился коридор)
		return 0;

	
}