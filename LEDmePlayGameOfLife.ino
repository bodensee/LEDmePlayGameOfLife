//
// Conway's Game of Life
//
// for the LEDmePlay
//
// by Daniel Hirscher 2016
//
// See: https://en.wikipedia.org/wiki/Conway's_Game_of_Life
// See also: http://rosettacode.org/wiki/Conway's_Game_of_Life
//

#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library

// Zeit zwischen Frames
#define interval 150

#define CLK 50
#define LAT 10
#define OE  51
#define A   A0
#define B   A1
#define C   A2
#define D   A3

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, true);

#define w 32
#define h 32

#define for_x for (int x = 0; x < w; x++)
#define for_y for (int y = 0; y < h; y++)
#define for_xy for_x for_y

uint16_t colorAdult = matrix.Color333(7, 7, 0);
uint16_t colorNewborn = matrix.Color333(3, 7, 0);
// Aktuelle Matrix
unsigned m[w][h];
// Matrix der nächsten Generation
unsigned b[w][h];

// Nachbarn: l=links, r=rechts, o=oben, u=unten; n = Anzahl der Nachbarn
int l, r, o, u, n;

unsigned long endtime = 0;
unsigned long waittime = 0;
unsigned long t;

void setup() {
  // nur einen Analog-Input auszulesen liefert manchmal den gleichen Wert,
  // deshalb werden mehrere miteinander verrechnet
  randomSeed(analogRead(7) * analogRead(8) + analogRead(9));
  matrix.begin();
}

void loop() {
  t = millis();
  if (t > waittime)
  {
    waittime = t + interval;

    // jede Minute neu starten, falls sich ein statisches Bild ergibt (oder nur Blinker)  
    if (t > endtime)
    {
      for_xy m[x][y] = random(2);
      endtime = t + 60000;
    }

    // evolve: nächste Generation berechnen
    for_xy {
      l = (x - 1 + w) % w;
      r = (x + 1 + w) % w;
      o = (y - 1 + h) % h;
      u = (y + 1 + h) % h;
      n = m[l][o] + m[x][o] + m[r][o] + m[l][y] + m[r][y] + m[l][u] + m[x][u] + m[r][u];
      b[x][y] = (n == 3 || (n == 2 && m[x][y]));
    }

    matrix.fillScreen(0);

    // umkopieren und zeichnen
    for_xy {
      if (b[x][y])
      {
        if (b[x][y] > m[x][y])
        {
          matrix.drawPixel(x, y, colorNewborn);
        }
        else
        {
          matrix.drawPixel(x, y, colorAdult);
        }
      }
      m[x][y] = b[x][y];
    }

    matrix.swapBuffers(false);
  }
}
