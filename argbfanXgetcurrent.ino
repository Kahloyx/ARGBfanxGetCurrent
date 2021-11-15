/*	This sketch is just a modification of an existing sketch that I found on internet, I kept it but I can't remind where I originally found it.
*/

#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;    // Have introduced the ina 219 part

#include <FastLED.h>
int argbSwitch = 2;    // This pin is used to power the argb led ring of the desktop PC fan
int currpal = 4;        // This one is the palette by default (FouthPalette())
int maxPalette = 4;     // This is the number of registrered palette I got into the sketch

#define LED_PIN     5       // The data pin of the argb led strip
#define NUM_LEDS    14		//Ofc it depends of your led strip
#define BRIGHTNESS  255		// Decimal value between 0 and 255...
#define LED_TYPE    WS2811	// Ofc it depends of your led strip
#define COLOR_ORDER GRB		// Same ...
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 60 // I used 42 for the secondPalette() 2520/42=60

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;
int i = 0; //This int is just giving me a start for measuring with the INA219 once it reaches 200 cycles (In this precise sketch)

void setup() {
  delay( 200 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
  // Just introducing the getcurrent setup
  Serial.begin(115200);
  while (!Serial) {
    // will pause Zero, Leonardo, etc until serial console opens
    delay(1);
  }

  uint32_t currentFrequency;
	pinMode(argbSwitch, OUTPUT);  // This brings power on the + pin of the argb led strip
  int j = 0;
  while (j < 2) {               // Just an init as we love'em
    fill_solid( currentPalette, 16, CRGB::White);
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */
    FillLEDsFromPaletteColors( startIndex);
    FastLED.show();
	  digitalWrite(argbSwitch, HIGH);
    FastLED.delay(5040 / UPDATES_PER_SECOND);
    digitalWrite(argbSwitch, LOW);   // And then we shut it off, init thing ya know
    delay(420);
    j++;
  }
  Serial.println("Hello!");

  // Initialize the INA219.
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range (see comments).
  if (! ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) {
      delay(10);
    }
  }
  // To use a slightly lower 32V, 1A range (higher precision on amps):
  //ina219.setCalibration_32V_1A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  ina219.setCalibration_32V_1A();

  Serial.println("Measuring voltage and current with INA219 ...");
  digitalWrite(argbSwitch, HIGH);           // This means that once the init done we light the argb on on the default palette
}

void loop() {
  if (i==200) // Each time i reach 200 it reads the power consumption over the INA219.
  {
    getcurrent();
    i=0;
  }
  i++;

  
  if (currpal == 1){
    FirstPalette();
  }
  if (currpal == 2){
    SecondPalette();
  }
  if (currpal == 3){
    ThirdPalette();
  }
  if (currpal == 4){
    FourthPalette();
  }
  if (currpal > maxPalette){    // This way we cycle throught the palette we got			/* Spaghetti code but I can't find better option ATM */
    currpal = 1;
  }


  static uint8_t startIndex = 0;     //Displaying the palette we've choosen
  startIndex = startIndex + 1; /* motion speed */

  FillLEDsFromPaletteColors( startIndex);

  FastLED.show();
  FastLED.delay(2520 / UPDATES_PER_SECOND);

  if ( Serial.available()) {     // Check if any char available on serial, if there's any instructions
    char c = Serial.read();
    if (c == 's'){			// Order to change to the next palette
      Serial.println("Switching.");
      Serial.println(c);
      currpal = currpal +1;
    }
if (c == 'n'){				// Order to power on the argb led strip 
      digitalWrite(argbSwitch, HIGH);
    }
    if (c == 'f'){			// Order to power off the argb led strip
      digitalWrite(argbSwitch, LOW);
    }
  }
}
