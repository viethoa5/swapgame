#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <fstream>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <cstdlib>
#include <ctime>

using namespace std;


const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 600;
const int WINDOW_CELL_WIDTH     =  300;
const int WINDOW_CELL_HEIGHT    =  200;
const SDL_Color White           = {255, 255, 255, 255};
const int DEFAULT_NUM_ROWS      = 2;
const int DEFAULT_NUM_COLS      = 5;
const string word_Path          = "word.txt";
const string fontpath           = "circle3d.ttf";
const string musicPath          = "Unstoppable.mp3";
const string MENUPATH           = "menu.png";
const string SCREEN_TITLE       = "swapword";
const string BACKGROUNDPATH     = "background.png";
const string PLAYPATH           = "play.png";
const string QUITPATH           = "quit.png";
const string WINPATH            = "win.png" ;

// SDL_Texture* createTexture : hàm chuyển .png thành texture
// shuffer : hàm làm đảo lộn chữ cái của từ tiếng anh
// choseword : hàm chọn ra 1 từ tiếng anh ngẫu nhiên trong 1 tập
// initGame : hàm  khởi tạo dữ liệu liên quan đến game : bảng, từ , trạng thái , vị trí
// initGraphic : hàm khởi tạo đồ họa và âm nhạc
// finalizeGraphic : hàm xóa đồ họa  và âm nhạc
// drawWord        : chuyển đổi string thành texture
// drawMenu        : vẽ Menu
// drawGame        : vẽ Game;
// gameevvent      : sự kiện xảy ra trong game(việc thay đổi vị trí chữ cái thông qua chuột)
// updategame      : cập nhật dữ liệu sau khi có sự kiện
// checkfullword   : kiểm tra từ tiếng anh đã đúng là từ cần tìm chưa
enum GameState {
    GAME_PLAYING,
    GAME_WON
};


enum LetterState {
  Letter_HIDDEN,
  Letter_SHOWN
};
struct Words{
  char letter;
  LetterState state;
};
struct WordPos {
    int row;
    int col;
};
struct Game {
    int nRows;
    int nCols;
    Words word[2][5];
    WordPos Pos[2];
    GameState state;
};
struct Graphic {
    SDL_Window *window;
    SDL_Texture *MENU;
    SDL_Texture *PLAY;
    SDL_Texture *WIN;
    SDL_Texture *QUIT;
    SDL_Texture *background;
    TTF_Font* font;
    SDL_Texture* text1;
    SDL_Texture* text2;
    Mix_Music* music;
    SDL_Renderer *renderer;
	SDL_Event event;
	SDL_Event menuevent;
};
SDL_Texture* createTexture(SDL_Renderer *renderer, const string &path) {
    SDL_Surface *surface = IMG_Load(path.c_str());
    if (surface == NULL) {
         cout <<"Unable to load image " << path << "!";
        return NULL;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}
void shuffle(string &s) {
 for (int i = s.length() -1; i > 0; i--) {
    int j = rand() % (i+1);
    swap(s[i],s[j]);
 }
}
int choseword(int n) {
   srand((unsigned) time(NULL));
   int res =  rand() % n;
   return res;
}
void initGame(Game &game, int nRows, int nCols) {
    game.nRows = nRows;
    game.nCols = nCols;
    game.state = GAME_PLAYING;
    ifstream wd (word_Path);
    string s[nCols],h;
    if (wd)
    {
        for (int i = 0; i < nCols; i++) wd >> s[i];
        wd.close();
    }
    else cout << "can't read file" << endl;
    h = s[choseword(nCols)];
    for(int i = 0; i < nCols; i++) {
        game.word[1][i].letter = h[i];
        game.word[1][i].state  = Letter_HIDDEN;
     }
     shuffle(h);
    for(int i = 0; i < nCols; i++) {
        game.word[0][i].letter = h[i];
        game.word[0][i].state  = Letter_SHOWN;
     }
}
bool initGraphic(Graphic &g) {
    g.window = NULL;
    g.renderer = NULL;

    int sdlFlags = SDL_INIT_VIDEO;
    if (SDL_Init(sdlFlags) != 0) {
        cout << "SDL could not initialize!";
        return false;
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        cout << "SDL_Image could not initialize!";
        return false;
    }

    g.window = SDL_CreateWindow(SCREEN_TITLE.c_str(),
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                SCREEN_WIDTH,
                                SCREEN_HEIGHT,
                                SDL_WINDOW_SHOWN);

    if (g.window == NULL) {
          cout <<"Window could not be created!";
        return false;
    }

    g.renderer = SDL_CreateRenderer(g.window, -1, SDL_RENDERER_ACCELERATED);
    if (g.renderer == NULL) {
          cout << "Renderer could not be created!";
        return false;
    }
    g.MENU           = createTexture(g.renderer, MENUPATH);
    g.PLAY           = createTexture(g.renderer, PLAYPATH);
    g.QUIT           = createTexture (g.renderer, QUITPATH);
    g.WIN            = createTexture(g.renderer, WINPATH) ;
    g.background     = createTexture(g.renderer, BACKGROUNDPATH);
    if (g.background == NULL) {
          cout <<"Unable to create texture from " <<" " <<  BACKGROUNDPATH << "!";
        return false;
    }
    if (g.MENU == NULL) {
          cout <<"Unable to create texture from " <<" " <<  MENUPATH << "!";
        return false;
    }
    if (g.PLAY == NULL) {
          cout <<"Unable to create texture from " <<" " <<  PLAYPATH << "!";
        return false;
    }
    if (g.WIN == NULL) {
          cout <<"Unable to create texture from " <<" " <<  WINPATH << "!";
        return false;
    }
    if (g.QUIT == NULL) {
          cout <<"Unable to create texture from " <<" " <<  QUITPATH << "!";
        return false;
    }
    if (TTF_Init() != 0)
    {
      cout << "Unable to create texture from " << " " << word_Path << "!";
        return false;
    }
    g.font = TTF_OpenFont(fontpath.c_str(), 26);
    if (g.font == NULL)
    {
        cout << "Unable to create font" << " " << "!";
        return false;
    }
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    g.music = Mix_LoadMUS(musicPath.c_str());
   return true;
}
void finalizeGraphic(Graphic &g) {
    SDL_DestroyRenderer(g.renderer);
    SDL_DestroyWindow(g.window);
    Mix_FreeMusic(g.music);
    IMG_Quit();
    SDL_Quit();
    Mix_CloseAudio;
}
void drawWord(Graphic& g,Game &game) {
    SDL_Surface* sur = NULL;
    string s,h;
    for(int i = 0; i < game.nCols; i++) {
        if(game.word[0][i].state == Letter_SHOWN) {
            s = s + game.word[0][i].letter;
        }
        else s = s + '*';
    }

    sur = TTF_RenderText_Solid(g.font, s.c_str(), White); // create text from surface
    g.text1 = SDL_CreateTextureFromSurface(g.renderer, sur); // turn text into texture
    for(int i = 0; i < game.nCols; i++) {
        if(game.word[1][i].state == Letter_SHOWN) {
            h = h + game.word[1][i].letter;
        }
        else h = h + '*';
    }
    sur = TTF_RenderText_Solid(g.font, h.c_str(), White);
    g.text2 = SDL_CreateTextureFromSurface(g.renderer, sur);
    SDL_FreeSurface(sur);
}
void drawMENU(Graphic& g) {
    SDL_RenderClear(g.renderer);
    SDL_RenderCopy(g.renderer, g.MENU, NULL, NULL);
    SDL_Rect srec = {500, 100, WINDOW_CELL_WIDTH, WINDOW_CELL_HEIGHT};
    SDL_RenderCopy(g.renderer, g.PLAY, NULL, &srec);
            srec = {500,  400, WINDOW_CELL_WIDTH, WINDOW_CELL_HEIGHT};
    SDL_RenderCopy(g.renderer, g.QUIT, NULL, &srec);
    SDL_RenderPresent(g.renderer);
}
void drawGame(Game& game,Graphic& g)
{
    SDL_RenderClear(g.renderer); // Clear the render before drawing again
    SDL_RenderCopy(g.renderer, g.background, NULL, NULL); // draw background
    SDL_Rect srec = {500, 100, WINDOW_CELL_WIDTH, WINDOW_CELL_HEIGHT};
    drawWord(g,game);
    SDL_RenderCopy(g.renderer, g.text1, NULL, &srec);
             srec = {500, 200, WINDOW_CELL_WIDTH, WINDOW_CELL_HEIGHT};
    SDL_RenderCopy(g.renderer, g.text2, NULL, &srec);
    SDL_DestroyTexture(g.text1);
    SDL_DestroyTexture(g.text2);
    if(game.state == GAME_WON) {
        srec = {500, 0, WINDOW_CELL_WIDTH, WINDOW_CELL_HEIGHT};
        SDL_RenderCopy(g.renderer, g.WIN, NULL, &srec);
   }
    SDL_RenderPresent(g.renderer);
}
void updateGame(Game &game, WordPos &Pos1, WordPos &Pos2){
  int i = 0;
  if(game.word[Pos1.row][Pos1.col].letter == game.word[Pos2.row+1][Pos2.col].letter) {
      game.word[Pos1.row][Pos2.col].state = Letter_HIDDEN;
      game.word[Pos2.row+1][Pos2.col].state = Letter_SHOWN;
      if(game.word[Pos1.row+1][Pos1.col].letter == game.word[Pos2.row][Pos2.col].letter) {
      game.word[Pos1.row][Pos1.col].state = Letter_HIDDEN;
      game.word[Pos1.row+1][Pos1.col].state = Letter_SHOWN;
      i = 1;
  }
  swap(game.word[Pos1.row][Pos1.col].letter,game.word[Pos2.row][Pos2.col].letter);
  }
 else {
    if(i == 0 && game.word[Pos1.row+1][Pos1.col].letter == game.word[Pos2.row][Pos2.col].letter) {
      game.word[Pos1.row][Pos1.col].state   = Letter_HIDDEN;
      game.word[Pos1.row+1][Pos1.col].state = Letter_SHOWN;
      swap(game.word[Pos1.row][Pos1.col].letter,game.word[Pos2.row][Pos2.col].letter);
  }
 }
}
void Gameevent(Game &game, const SDL_Event &event, long &n){
    SDL_MouseButtonEvent mouse = event.button;
    if (event.type != SDL_MOUSEBUTTONDOWN) {
        return;
    }
    int col = (mouse.x-500) / (WINDOW_CELL_WIDTH /5);
    int row = 0;
       if(n %2 == 0) {
        game.Pos[0].row = row;
        game.Pos[0].col = col;
        }
        if(n %2 != 0) {
        col = (mouse.x-500) / (WINDOW_CELL_WIDTH /5);
        game.Pos[1].row = row;
        game.Pos[1].col = col;
       updateGame(game, game.Pos[0], game.Pos[1]);
        }
       n++;
}
void checkfullword(Game &game) {
 int j = 0;
 for(int i = 0; i < game.nCols; i++) {
    if(game.word[1][i].state != Letter_SHOWN ) {
        j = 1;
        break;
    }
 }
 if(j == 0) {
        game.state = GAME_WON;
 }
}
int menuevent(Graphic &g, const SDL_Event &event, bool & quit) {
    SDL_MouseButtonEvent mouse = event.button;
    if(event.type == SDL_MOUSEBUTTONDOWN) {
    if(mouse.y >= 100 && mouse.y <= 300) {
        if(mouse.x >= 500 && mouse.x <= 800) {
              return 1;
        }
    }
    if(mouse.y >= 400 && mouse.y <= 600) {
        if(mouse.x >= 500 && mouse.x <= 800) {
                quit = true;
                return 0;
        }
    }

}
}




int main(int agrc, char* agrv[]) {
   srand(time(0));
   int menulevel = 2;
    int nRows = DEFAULT_NUM_ROWS,
        nCols = DEFAULT_NUM_COLS;

    Graphic g;
    Game  game;
    if (!initGraphic(g)) {
        finalizeGraphic(g);
        return EXIT_FAILURE;
    }
    bool quit = false;
   while (!quit) {
          bool quitplay  = false;
          long i = 0;
          SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
            drawMENU(g);
            if (event.type == SDL_QUIT) {
                quit = true;
                break;
            }
             menulevel= menuevent(g,event,quit);
      if(menulevel == 1) {
            Mix_PlayMusic(g.music, -1);
            initGame(game, nRows, nCols);
            g.font = TTF_OpenFont(fontpath.c_str(), 26);
       while(!quitplay) {
             drawGame(game,g);
             checkfullword(game);
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
               quitplay = true;
                break;
            }
            Gameevent(game,event,i);
        }
    }

}
}
}
    finalizeGraphic(g);
    return EXIT_SUCCESS;
}





