#include <Metro.h>
#include <wavTrigger.h>
#include <AltSoftSerial.h>

Metro seqMetro(1000);        // Sequencer state machine interval timer
Metro readAnalogInputs(100);      // Read potentiometer values
Metro readDigitalInputs(100);     // Read switch values

//WAV Trigger will only play WAV files formatted as 16-bit, stereo, 44.1kHz, and there can be no meta-data (non-audio data) in the file before the audio data.
//Make sure the file name starts with a 3 digit number.
wavTrigger wTrig;             // Our WAV Trigger object

// ------------------------------------------------------------------    DECLARE PINS AND CONSTANTS ------------------------------------------------------------//
const int tempoPin = A0;
const int volumePin = A1;

const int pauseSwitchPin = 2;
const int swingSwitchPin = 3;
const int stutterSwitchPin = 4;

//--------------------------------------------------------------VARIABLE DECLARATION & INITIALIZATION------------------------------------------------------//
boolean swingOn = false;
boolean stutterOn = false;
boolean pauseOn = false;

int tempoMS;
int volume;
int swingMS = 0;
int tempoBPM;

word beatMaster = 0;

//----------------------------------------------------------------------------------PATTERN CREATION-----------------------------------------------------------------------------//
const word hat_p1 PROGMEM = 0b1010101010101010;
const word kick_p1 PROGMEM = 0b1000100010001000;
const word snare_p1 PROGMEM = 0b1010101010101011;

const word pattern1[] PROGMEM = {hat_p1, kick_p1, snare_p1};

const word hat_p2 PROGMEM = 0b1111111100000000;
const word kick_p2 PROGMEM = 0b1111111100000000;
const word snare_p2 PROGMEM = 0b1111111100000000;

const word pattern2[] PROGMEM = {hat_p2, kick_p2, snare_p2};

// this class contains all the information contained in 1 Drummer Boy bar of music excluding sample volumes which are only stored/restored when saving/loading
class ActivePattern
{
  public:
    word hat = 0b0000000000000000;
    word kick = 0b0000000000000000;
    word snare = 0b0000000000000000;
};

ActivePattern pattern;

//------------------------------------------------------------------- FUNCTION PROTOTYPES--------------------------------------------------------------------------------------------------//

// FUNCTION PROTOTYPES // -  The Arduino IDE creates function prototypes for you. Normally, this works quite well. There are some situations, like functions with reference arguments, where it doesn't.
void checkAnalogIO();
void checkDigitalIO();
void stutter();


// ***************************************SETUP*******************************************************
void setup() {
  // Serial monitor for debugging
  Serial.begin(9600);

  Serial.println("hello world");

  delay(100);

  switchPattern(1);

//  word mybuffer = 0;
//  for (int i = 0; i < 3; i++) {
//    word testing = pgm_read_word( &pattern1[i] );
//    Serial.println(testing, BIN);
//    delay(500);
//  }

  
//  
//
//  pinMode(pauseSwitchPin, INPUT_PULLUP);
//  pinMode(swingSwitchPin, INPUT_PULLUP);
//  pinMode(stutterSwitchPin, INPUT_PULLUP);
//
//  delay(500); // wait for the WAV Trigger to finish reset before trying to send commands.
//  wTrig.start(); // WAV Trigger startup at 57600
//  delay(10);
//  wTrig.stopAllTracks();// Send a stop-all command and reset the sample-rate offset, in case we have reset while the WAV Trigger was already playing.
//  wTrig.samplerateOffset(0); // make sure the pitch is unmodified
//
//  // enable track reporting (recieve data from the WAV Trigger)
//  wTrig.setReporting(true);
//  delay(100);
//  // get version/track info from the wav trigger
//  char gWTrigVersion[VERSION_STRING_LEN];    // WAV Trigger version string
//  if (wTrig.getVersion(gWTrigVersion, VERSION_STRING_LEN)) {
//    Serial.print(gWTrigVersion);
//    Serial.print("\n");
//    int gNumTracks = wTrig.getNumTracks();
//    Serial.print("Number of tracks = ");
//    Serial.println(gNumTracks);
//  }
//  else
//    Serial.print("WAV Trigger response not available");
//
//  delay(1000);
//
//  readTempo(1); // read vol and tempo values
//  readVolume(1);
//  wTrig.trackPlayPoly(200);   // play startup sound

}

//------------------------------------------------------------------------------------MAIN LOOP------------------------------------------------------------------------//

void loop() {

  if (!pauseOn) {
    if (seqMetro.check() == 1) {
      setSwing();
      for (int i = 0; i < 8 ; i++) {
        wTrig.trackPlayPoly(11);
      }
    }
    
    if (stutterOn) stutter();

  } else {
    if (digitalRead(pauseSwitchPin) == HIGH && pauseOn) {
      pauseOn = false;
      seqMetro.reset();
      startAtBeat0();
    }
    
  }
//  checkDigitalIO();
//  checkAnalogIO();
}

//------------------------------------------------------------------------------------LOOP METHODS------------------------------------------------------------------------//

void switchPattern(int p) {
  if(p == 1) transferPatternMem(pattern1);
  if(p == 2) transferPatternMem(pattern2);
}

void transferPatternMem(const word memPattern[]){
  pattern.hat = pgm_read_word( &memPattern[0] );
  pattern.kick = pgm_read_word( &memPattern[1] );
  pattern.snare = pgm_read_word( &memPattern[2] );
}

void stutter() {
  beatMaster = 14;
}

void startAtBeat0() {
  beatMaster = 0;
}

void incrementBeatCounters() {
  beatMaster++;
}

void setSwing() {
  if (swingOn) {
    if (beatMaster % 2 == 1)seqMetro.interval(tempoMS - swingMS);
    else seqMetro.interval(tempoMS + swingMS);
  }
}

//----------------------------------------------------IO SCAN--------------------------------------------------------------------------------/
//---------------------- Analog -------------------//
void checkAnalogIO() {
  if (readAnalogInputs.check() == 1) {  // if minumum time has passed the inputs are read
    readTempo(0);
    readVolume(0);
  }
}

void readVolume(boolean firstCall) {      // read analog input for volume and map to wav trigger vol range (-70 to +4)
  static int volumeLastIntermediate;
  static int volumeIntermediate;
  volumeIntermediate = analogRead(volumePin);
  if ( abs( volumeLastIntermediate - volumeIntermediate) > 10 || firstCall) {
    volumeLastIntermediate = volumeIntermediate;
    volume = map(volumeIntermediate, 0, 1023, -70, 4);  // -70 to 4 DB is the full range of the wav trigger
    wTrig.masterGain(volume);
    Serial.print("Volume: "); Serial.println(volume);
  }
}

void readTempo(boolean firstCall) {
  // read analog input for tempo - if significantly changed update the tempo
  static int tempoIntermediate;
  static int tempoLastIntermediate;
  tempoIntermediate = analogRead(tempoPin);
  if ( abs( tempoLastIntermediate - tempoIntermediate ) > 10 || firstCall) {
    tempoLastIntermediate = tempoIntermediate;
    tempoMS = map(tempoIntermediate, 1023, 0, 83, 320);
    seqMetro.interval(tempoMS);
    tempoBPM = 60000 / (tempoMS * 4);   /// ms to BPM conversion, the 4 is there because every DB "beat" is actually an eight note
  }
}

//---------------------- Digital -------------------//
void checkDigitalIO() {
  if (readDigitalInputs.check() == 1) {  // if minumum time has passed the inputs are read
    readPauseSwitch();
    readSwingSwitch();
    readStutterSwitch();
  }
}

void readSwingSwitch() {
  if ( digitalRead(swingSwitchPin) == LOW && !swingOn) {
    Serial.println("Swing ON");//
    swingOn = 1;
  }
  if (digitalRead(swingSwitchPin) == HIGH && swingOn) {
    Serial.println("Swing OFF");
    seqMetro.interval(tempoMS);
    swingOn = 0;
    swingMS = 0; //reset swing rate to 0 while switch is off
  }
}

void readStutterSwitch() {
  if ( digitalRead(stutterSwitchPin) == LOW && !stutterOn) {
    stutterOn = true;
    Serial.println("stutter ON");
  }
  if (digitalRead(stutterSwitchPin) == HIGH && stutterOn && beatMaster == 15) {
    stutterOn = false;
    Serial.println("stutter OFF");
  }
}

void readPauseSwitch() {
  if ( digitalRead(pauseSwitchPin) == LOW && !pauseOn) {
    pauseOn = true;
    Serial.println("pause ON");
  }
  if (digitalRead(pauseSwitchPin) == HIGH && pauseOn) {
    pauseOn = false;
    Serial.println("pause OFF");
  }
}
