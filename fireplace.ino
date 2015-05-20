#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include "RGB.h"

#define PS_PIXELS_Y 8
#define PS_PIXELS_X 8
#define PS_MAX_X 300
#define PIN 1
#define FADE 4
#define PARTICLE_COUNT 100

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
  NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);

#include "ParticleSys.h"
#include "Particle_Std.h"
#include "Particle_Fixed.h"
#include "Particle_Attractor.h"
#include "Emitter_Fire.h"
#include "Emitter_Fountain.h"
#include "PartMatrix.h"

boolean pulseOn = false;

Particle_Std particles[PARTICLE_COUNT];
Particle_Fixed source;
Emitter_Fire emitter;
ParticleSys pSys(PARTICLE_COUNT, particles, &emitter);
PartMatrix pMatrix;

void setup() {
  randomSeed(analogRead(0));
  
  matrix.begin();
  matrix.setBrightness(40);
  
  emitter.cycleHue = false;
  
  source.hue = 0;
  source.vx = 0;
  source.vy = 0;
  source.x = 150;
  source.y = 0;
//  Particle_Attractor::atf = 2;
  
  Emitter_Fire::baseHue = 10;
  Emitter_Fountain::minLife = 40;
  Emitter_Fountain::maxLife = 250;
  ParticleSys::perCycle = 1;
  Emitter_Fire::maxTtl = 96;
  Particle_Std::ay = 1;
  PartMatrix::isOverflow = true;

  //init all pixels to zero
  pMatrix.reset();
}


/**
 * Render the particles into a low-resolution matrix
 */
void drawMatrix(){
    pMatrix.fadeBy(FADE);
    pMatrix.render(particles, PARTICLE_COUNT);
    
    //update the actual LED matrix
    for (byte y=0;y<PS_PIXELS_Y;y++) {
        for(byte x=0;x<PS_PIXELS_X;x++) {
            matrix.drawPixel(x, y, matrix.Color(pMatrix.matrix[x][y].r, pMatrix.matrix[x][y].g, pMatrix.matrix[x][y].b));
        }
    }
    matrix.show();
}
  
void loop() {  
  pSys.update();  
  drawMatrix();
  delay(10);
}

