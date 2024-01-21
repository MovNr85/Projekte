#include <array>
#include <iostream>
#include <string>
#include <utility>
#include <thread>
#include <vector>

#include <boost/multi_array.hpp>


unsigned long const n = 8;


typedef boost::multi_array<int,2> array_type;


// Struct um die Positionen zu speichern und If-Check vereinfachen.
struct Vector2D
{

  int x = 0, y = 0;

  bool operator<(int var) { return (x < var) & (y < var); }

  bool operator>(int var) { return (x > var) & (y > var); }
};

// Tabelle mit den möglichen Zügen des Springers.
std::array<std::array<int, 2>, 8> g_kSprungTabelle = {
    {{1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}}};

// Funktion zur ausgabe der Lösung.
void Ausgabe(array_type const &kFeld)
{

  for (auto Reihe : kFeld)
  {

    for (auto Nummer : Reihe)
      printf("|%2i", Nummer);

    printf("|\n");
  }
  printf("\n");
}


int Backtrack(array_type const &Feld,
              unsigned const &Sprung, Vector2D const &Springer)
{


  // Ausgabe der Lösungen
  if (Sprung == n * n)
  {

    Ausgabe(Feld);
    exit(0);
  }

  for (unsigned j = 0; j < 8; j++)
  {
    {
      Vector2D SpringerTemp{Springer.x + g_kSprungTabelle[j][0],
                            Springer.y + g_kSprungTabelle[j][1]};

      // Check ob Springer im Feld und Poisition im Feld nicht beschrieben ist.
      if ((SpringerTemp > -1) & (SpringerTemp < n))
      {
        if (Feld[SpringerTemp.x][SpringerTemp.y] == 0)
        {

          // Kopieren von Feld und Sprung
          auto FeldTemp = Feld;
          auto SprungTemp = Sprung + 1;

          // Sprung im Feld eintragen
          FeldTemp[SpringerTemp.x][SpringerTemp.y] = SprungTemp;

          // Neue Iteration starten.
          Backtrack(FeldTemp, SprungTemp, SpringerTemp);
        }
      }
    }
  }

  // Wenn kein weiterer Sprung möglich dann beenden.
  return 0;
}

int main(int argc, char *argv[])
{

  // Spielbrett vorbereiten.
  array_type Feld{boost::extents[n][n]};


  // Position des Springers setzen.

  int x = 0;
  int y = 0;

  if (argc > 2)
  {
    x = std::atoi(argv[1]);
    y = std::atoi(argv[2]);
  }

  // Startposition auf dem Spielbrett setzen und Springer Object erstellen.
  Feld[x][y] = 1;
  Vector2D Springer{x, y};

  // Das Backtracking starten.
  Backtrack(Feld, 1, Springer);

  return 0;
}
