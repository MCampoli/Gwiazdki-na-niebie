#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>

using namespace std;

const int SZEROKOSC_EKRANU = 800;
const int WYSOKOSC_EKRANU = 600;

SDL_Window* Okno = nullptr;
SDL_Renderer* Render = nullptr;
SDL_Texture* TeksturaGwiazdki = nullptr;

struct Gwiazda {
    int x, y;
};

vector<Gwiazda> gwiazdy;

// Inicjalizacja SDL
bool inicjalizujSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "SDL nie moglo siê zainicjowac! Blad SDL: " << SDL_GetError() << endl;
        return false;
    }

    Okno = SDL_CreateWindow("Gwiazdki na niebie", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SZEROKOSC_EKRANU, WYSOKOSC_EKRANU, SDL_WINDOW_SHOWN);

    if (Okno == nullptr) {
        cout << "Nie udalo sie utworzyc okna! Blad SDL: " << SDL_GetError() << endl;
        return false;
    }

    Render = SDL_CreateRenderer(Okno, -1, SDL_RENDERER_ACCELERATED);

    if (Render == nullptr)
        {
        cout << "Nie udalo siê utworzyc renderera! Blad SDL: " << SDL_GetError() << endl;
        return false;
    }

    return true;
}

// Wczytywanie tekstury gwiazdki
SDL_Texture* wczytajTekstureGwiazdki()
{
    SDL_Surface* powierzchniaGwiazdki = IMG_Load("gwiazdka.png");

    if (powierzchniaGwiazdki == nullptr) {
        cout << "Nie mozna wczytac obrazu gwiazdki! Blad SDL_image: " << IMG_GetError() << endl;
        return nullptr;
    }

    SDL_Texture* tekstura = SDL_CreateTextureFromSurface(Render, powierzchniaGwiazdki);

    if (tekstura == nullptr)
        {
        cout << "Nie mozna utworzyc tekstury z powierzchni gwiazdki! Blad SDL: " << SDL_GetError() << endl;
    }

    SDL_FreeSurface(powierzchniaGwiazdki);
    return tekstura;
}

// Renderowanie gwiazdek na ekranie
void rysujGwiazdy()
{
    for (const auto& gwiazda : gwiazdy) {
        SDL_Rect destRect = { gwiazda.x, gwiazda.y, 32, 32 };
        SDL_RenderCopy(Render, TeksturaGwiazdki, nullptr, &destRect);
    }
}

// Zwolnienie zasobów i zamkniêcie bibliotek SDL i SDL_image
void zamknijProgram()
{
    SDL_DestroyTexture(TeksturaGwiazdki);
    SDL_DestroyRenderer(Render);
    SDL_DestroyWindow(Okno);
    IMG_Quit();
    SDL_Quit();
}

int WinMain(int argc, char* argv[])
{
    if (!inicjalizujSDL())
        {
        return 1;
    }

    TeksturaGwiazdki = wczytajTekstureGwiazdki();

    SDL_Event e;
    bool koniecProgramu = false;

    while (!koniecProgramu)
        {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT) {
                koniecProgramu = true;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                if (e.button.button == SDL_BUTTON_LEFT)
                {
                    Gwiazda nowaGwiazda = { e.button.x, e.button.y };
                    gwiazdy.push_back(nowaGwiazda);//Rysowanie gwiazdek lewym przyciskiem myszy
                }
                else if (e.button.button == SDL_BUTTON_RIGHT) {
                    gwiazdy.clear(); // Czyszczenie gwiazdek
                }
            }
        }

        // Czyszczenie ekranu na jasnoniebieski kolor
        SDL_SetRenderDrawColor(Render, 173, 216, 230, 255);
        SDL_RenderClear(Render);

        // Rysowanie gwiazdek
        rysujGwiazdy();

        // Renderowanie
        SDL_RenderPresent(Render);
    }

    // Zwolnienie zasobów przed zakoñczeniem programu
    zamknijProgram();

    return 0;
}
