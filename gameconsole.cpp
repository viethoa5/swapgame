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
const int rows = 2;
const int cols = 5;
// void clrsc : hàm xóa màn hình
// void creatboard : hàm tạo bảng
//// void shuffer : hàm thay đổi vị trí chữ cái trong bảng và giá trị của chữ cái đó
// void showboard : hàm xuất hình ảnh bảng ra màn hình
// void swapword : thao tác chuyển chữ cái từ vị trí ban đầu đến vị trí chuyển đến
// bool checkword : hàm kiểm tra chữ cái di chuyển có đến đúng vị trí thật sự của nó không
// bool checkfullword : hàm kiểm tra xem tất cả các chữ cái đã quay về đúng vọ trí chưa
// int choseword : hàm trả về giá trị ngẫu nhiên từ đó chọn đc xâu trong list


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


enum TrangThai {hidden,shown};


struct board{
int value; // giá trị của 1 ô
TrangThai tt; // trạng thái của ô đang hiện hay ẩn
char word;   // kí tự được chứa trong ô
};

struct Game{
board boards[20][20]; // số ô trong game
int rows; // số hàng
int cols; // số cột
};
void shuffle(Game & game) {
 for (int i = cols -1; i > 0; i--) {
    int j = rand() % (i+1); // hàm lấy ngẫu nhiên từ 0 -> i
    swap(game.boards[0][i].word,game.boards[0][j].word); // hoán đổi vị trí chữ cái
    swap(game.boards[0][i].value,game.boards[0][j].value); // hoán đổi vị trí giá trị của chữ cái
 }
}

void creatboard(string s1,Game & game){

for(int i = 0; i < cols; i++) { // gán chữ cái, giá trị , trạng thái của từng cột
    game.boards[0][i].word = s1[i];
    game.boards[1][i].word = s1[i];
    game.boards[1][i].value = i;
    game.boards[0][i].value = i;
    game.boards[1][i].tt=hidden;
}
for (int i = 0; i < cols -1; i++) {  // tránh trường hợp có chữ cái giống nhau trong bảng nhưng mang giá trị khác nhau
    for(int j = i+1; j < cols; j++) {
        if(game.boards[1][i].word == game.boards[1][j].word) {
            game.boards[1][j].value = game.boards[1][i].value;
            game.boards[0][j].value = game.boards[0][i].value;
        }
    }
}
     for(int i = 0; i < cols; i++) {
    game.boards[0][i].tt= shown; // đổi trạng thái của bảng
}
}
void showboard(Game &game){
    clrscr();
for (int i = 0;i < rows; i++) {
    for(int j = 0; j < cols; j++) {
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
    for(int j = 0; j < cols; j++) {
        if(game.boards[1][j].tt == hidden) {
            return false;
            break;
        }
    }
    return true;
}
void swapword(Game &game,int &num1, int &num2) {
    if (checkword(game,num1,num2) == true) {
        game.boards[1][num2-1].value = -1; // tránh trường hợp 2 chữ cái giống nhau cùng chuyển về 1 vị trí
        game.boards[1][num2-1].tt = shown;
        game.boards[0][num1-1].word = '*'; // đánh dấu là đã chuyển
    }
}
int choseword(int n) {
   srand((unsigned) time(NULL));
   int res =  rand() % n;
   return res;
}
int main() {
ifstream fileInput("C:/Users/ADMIN/Desktop/file.txt"); // mở file
if (fileInput.fail()) // mở file thất bại
	{
		std::cout << "Failed to open this file!" << std::endl;
		return -1;
	}
	int n;
	fileInput >> n; // nhập dữ liệu int từ text

    string word[n];// tập hợp các xâu
    string space;
    getline(fileInput, space); // đọc dấu cách
	for (int i = 0; i < n; i++) {
     fileInput >> word[i];// nhập các xâu
	}
	string wordguessed = word[choseword(n)];// chọn  ra  1 xâu ngẫu nhiên
  board boards;
  Game game;
  creatboard(wordguessed,game);
  shuffle(game);
  while(checkfullword(game) == false) {
    showboard(game);
    int num1,num2;
    cin >> num1 >> num2;
    swapword(game,num1,num2);
    }

  showboard(game);
  return 0;
}

