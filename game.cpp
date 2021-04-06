#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
// int choseword : chose a random word from list
// string shuffle : swap letter from word chosen
// void print : print word chosen
// string checkwordsimple : check the letter from word
// bool checkfullword : check full word
// string swapletter : swap letter from  swapped word to find find true word
using namespace std;
int choseword(int n) {
   int randomnum = rand() % n;
   return randomnum;
}
string shuffle(string wordguessed) {
 for (int i = wordguessed.length() -1; i >0; i--) {
    swap(wordguessed[i],wordguessed[i-1]);
 }
 return wordguessed;
}
void print(string s) {
    for (int i = 0; i < s.length(); i++) {
        cout << s[i];
    }
    cout << endl;
}
string checkwordsimple(string wordchange,string wordguess, int a, int b) {
    if (a > wordchange.length() || b > wordchange.length()) {
        return "wrong";
    }
    else {
     if (wordchange[a] == wordguess[b] || wordchange[b] == wordguess[a] ) {
        return "true";
     }
     else return "wrong";
    }

}
bool checkfullword(string s1, string s2) {
for (int i = 0; i < s1.length(); i++) {
    if (s1[i] != s2[i]) {
       return false;
       break;
    }
}
return true;
}

string swapletter(string wordchange,string wordguess) {
    int num1,num2;
    cin >> num1 >> num2;
    string check = checkwordsimple(wordchange,wordguess,num1,num2);
    if (check == "wrong") {
        print(check);
        swapletter(wordchange,wordguess);
    }
    else {
        swap(wordchange[num1],wordchange[num2]);
        print(wordchange);
       if(checkfullword(wordchange,wordguess) == 0) swapletter(wordchange,wordguess);
    }
   return "victory";
}



int main () {
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
	string wordchange = shuffle(wordguessed);
	while(checkfullword(wordchange,wordguessed) == 1) {
	   wordchange = shuffle(wordchange);
	}
   print(wordchange);
   wordchange = swapletter(wordchange,wordguessed);
   print(wordchange);
    return 0;
}
