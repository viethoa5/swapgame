#include<iostream>
#include<string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include<stdio.h>
#include<conio.h>
#include<time.h>
#include<Windows.h>
#include <fstream>
using namespace std;
const int rows = 5;
const int cols = 2;


void clrscr(void)
{
CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
HANDLE hConsoleOut;
COORD Home = {0,0};
DWORD dummy;

hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
GetConsoleScreenBufferInfo(hConsoleOut,&csbiInfo);

FillConsoleOutputCharacter(hConsoleOut,' ',csbiInfo.dwSize.X * csbiInfo.dwSize.Y,Home,&dummy);
csbiInfo.dwCursorPosition.X = 0;
csbiInfo.dwCursorPosition.Y = 0;
SetConsoleCursorPosition(hConsoleOut,csbiInfo.dwCursorPosition);
}
void gotoXY(short x,short y)
{
HANDLE hConsoleOutput;
COORD Cursor_an_Pos = { x,y};
hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
SetConsoleCursorPosition(hConsoleOutput , Cursor_an_Pos);
}

enum TrangThai {hidden,shown};
enum gameplay {wrong,win,lost,right};

struct board{
int value;
TrangThai tt;
char word;
};

struct Game{
board boards[1][5];
int rows;
int cols;
};
string shuffle(string wordguessed,Game & game) {
 for (int i = wordguessed.length() -1; i >0; i--) {
    int j = rand() % (i+1);
    swap(wordguessed[i],wordguessed[j]);
    swap(game.boards[0][i].value,game.boards[0][j].value);
 }
 return wordguessed;
}

void creatboard(string s1,Game & game){

for(int i = 0; i < rows; i++) {
    game.boards[1][i].word = s1[i];
    game.boards[1][i].value = i;
    game.boards[0][i].value = i;
    game.boards[1][i].tt=hidden;
}
     s1 = shuffle(s1,game);
     for(int i = 0; i < rows; i++) {
    game.boards[0][i].word = s1[i];
    game.boards[0][i].tt= shown;
}
}
void showboard(Game &game){
    clrscr();
for (int i = 0;i < cols; i++) {
    for(int j = 0; j < rows; j++) {
        if(game.boards[i][j].tt == shown) {
        cout << game.boards[i][j].word;
    }
    else cout << "_" ;
    }
    cout << endl;
  }
}

bool checkword(Game & game,int &num1,int &num2) {
   if (game.boards[0][num1-1].value == game.boards[1][num2-1].value) {
    return true;
   }
 return false;
}
bool checkfullword(Game & game) {
    for(int j = 0; j < rows; j++) {
        if(game.boards[1][j].tt == hidden) {
            return false;
            break;
        }
    }
    return true;
}
void swapword(Game &game,int &num1, int &num2) {
    if (checkword(game,num1,num2) == true) {
        game.boards[1][num2-1].tt = shown;
        game.boards[0][num1-1].word = '*';
    }
}
int choseword(int n) {
   srand((unsigned) time(NULL));
   int res =  rand() % n;
   return res;
}
int main() {
ifstream fileInput("C:/Users/ADMIN/Desktop/file.txt");
if (fileInput.fail())
	{
		std::cout << "Failed to open this file!" << std::endl;
		return -1;
	}
	int n;
	fileInput >> n;

    string word[n];
    string space;
    getline(fileInput, space);
	for (int i = 0; i < n; i++) {
     fileInput >> word[i];
	}
	string wordguessed = word[choseword(n)];
  board boards;
  Game game;
  creatboard(wordguessed,game);
  while(checkfullword(game) == false) {
    showboard(game);
    int num1,num2;
    cin >> num1 >> num2;
    swapword(game,num1,num2);
    }
   for(int i = 0 ; i < cols; i++) {
    cout << game.boards[0][i].value << " ";
   }

  showboard(game);
  return 0;

}



