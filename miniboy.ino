#include <Metro.h>
#include <wavTrigger.h>
#include <AltSoftSerial.h>

Metro seqMetro(1000);        // Sequencer state machine interval timer
Metro readAnalogInputs(100);      // Read potentiometer values
Metro readDigitalInputs(100);     // Read switch values

//WAV Trigger will only play WAV files formatted as 16-bit, stereo, 44.1kHz, and there can be no meta-data (non-audio data) in the file before the audio data.
//Make sure the file name starts with a 3 digit number.
wavTrigger wTrig;             // Our WAV Trigger object

//----------------------------------------------------------------------------------PATTERN CREATION-----------------------------------------------------------------------------//
// Use the congas patterns for toms
const word hatclosed_p1 PROGMEM = 0b0000000000000000;
const word hatopen_p1 PROGMEM = 0b0000000000000000;
const word rim_p1 PROGMEM = 0b0000000000000000;
const word shaker_p1 PROGMEM = 0b0000000000000000;
const word cymbal_p1 PROGMEM = 0b0000000000000000;
const word kick_p1 PROGMEM = 0b0000000000000000;
const word snare_p1 PROGMEM = 0b0000000000000000;
const word clap_p1 PROGMEM = 0b0000000000000000;
const word cowbell_p1 PROGMEM = 0b0000000000000000;
const word claves_p1 PROGMEM = 0b0000000000000000;
const word congahigh_p1 PROGMEM = 0b0000000000000000;
const word congamid_p1 PROGMEM = 0b0000000000000000;
const word congalow_p1 PROGMEM = 0b0000000000000000;
const word pattern1[] PROGMEM = {hatclosed_p1, hatopen_p1, rim_p1, shaker_p1, cymbal_p1, kick_p1, snare_p1, clap_p1, cowbell_p1, claves_p1, congahigh_p1, congamid_p1, congalow_p1};

const word hatclosed_p2 PROGMEM = 0b0000000000000000;
const word hatopen_p2 PROGMEM = 0b0000000000000000;
const word rim_p2 PROGMEM = 0b0000000000000000;
const word shaker_p2 PROGMEM = 0b0000000000000000;
const word cymbal_p2 PROGMEM = 0b0000000000000000;
const word kick_p2 PROGMEM = 0b0000000000000000;
const word snare_p2 PROGMEM = 0b0000000000000000;
const word clap_p2 PROGMEM = 0b0000000000000000;
const word cowbell_p2 PROGMEM = 0b0000000000000000;
const word claves_p2 PROGMEM = 0b0000000000000000;
const word congahigh_p2 PROGMEM = 0b0000000000000000;
const word congamid_p2 PROGMEM = 0b0000000000000000;
const word congalow_p2 PROGMEM = 0b0000000000000000;
const word pattern2[] PROGMEM = {hatclosed_p2, hatopen_p2, rim_p2, shaker_p2, cymbal_p2, kick_p2, snare_p2, clap_p2, cowbell_p2, claves_p2, congahigh_p2, congamid_p2, congalow_p2};

const word hatclosed_p3 PROGMEM = 0b0000000000000000;
const word hatopen_p3 PROGMEM = 0b0000000000000000;
const word rim_p3 PROGMEM = 0b0000000000000000;
const word shaker_p3 PROGMEM = 0b0000000000000000;
const word cymbal_p3 PROGMEM = 0b0000000000000000;
const word kick_p3 PROGMEM = 0b0000000000000000;
const word snare_p3 PROGMEM = 0b0000000000000000;
const word clap_p3 PROGMEM = 0b0000000000000000;
const word cowbell_p3 PROGMEM = 0b0000000000000000;
const word claves_p3 PROGMEM = 0b0000000000000000;
const word congahigh_p3 PROGMEM = 0b0000000000000000;
const word congamid_p3 PROGMEM = 0b0000000000000000;
const word congalow_p3 PROGMEM = 0b0000000000000000;
const word pattern3[] PROGMEM = {hatclosed_p3, hatopen_p3, rim_p3, shaker_p3, cymbal_p3, kick_p3, snare_p3, clap_p3, cowbell_p3, claves_p3, congahigh_p3, congamid_p3, congalow_p3};

const word hatclosed_p4 PROGMEM = 0b0000000000000000;
const word hatopen_p4 PROGMEM = 0b0000000000000000;
const word rim_p4 PROGMEM = 0b0000000000000000;
const word shaker_p4 PROGMEM = 0b0000000000000000;
const word cymbal_p4 PROGMEM = 0b0000000000000000;
const word kick_p4 PROGMEM = 0b0000000000000000;
const word snare_p4 PROGMEM = 0b0000000000000000;
const word clap_p4 PROGMEM = 0b0000000000000000;
const word cowbell_p4 PROGMEM = 0b0000000000000000;
const word claves_p4 PROGMEM = 0b0000000000000000;
const word congahigh_p4 PROGMEM = 0b0000000000000000;
const word congamid_p4 PROGMEM = 0b0000000000000000;
const word congalow_p4 PROGMEM = 0b0000000000000000;
const word pattern4[] PROGMEM = {hatclosed_p4, hatopen_p4, rim_p4, shaker_p4, cymbal_p4, kick_p4, snare_p4, clap_p4, cowbell_p4, claves_p4, congahigh_p4, congamid_p4, congalow_p4};

const word hatclosed_p5 PROGMEM = 0b0000000000000000;
const word hatopen_p5 PROGMEM = 0b0000000000000000;
const word rim_p5 PROGMEM = 0b0000000000000000;
const word shaker_p5 PROGMEM = 0b0000000000000000;
const word cymbal_p5 PROGMEM = 0b0000000000000000;
const word kick_p5 PROGMEM = 0b0000000000000000;
const word snare_p5 PROGMEM = 0b0000000000000000;
const word clap_p5 PROGMEM = 0b0000000000000000;
const word cowbell_p5 PROGMEM = 0b0000000000000000;
const word claves_p5 PROGMEM = 0b0000000000000000;
const word congahigh_p5 PROGMEM = 0b0000000000000000;
const word congamid_p5 PROGMEM = 0b0000000000000000;
const word congalow_p5 PROGMEM = 0b0000000000000000;
const word pattern5[] PROGMEM = {hatclosed_p5, hatopen_p5, rim_p5, shaker_p5, cymbal_p5, kick_p5, snare_p5, clap_p5, cowbell_p5, claves_p5, congahigh_p5, congamid_p5, congalow_p5};

const word hatclosed_p6 PROGMEM = 0b0000000000000000;
const word hatopen_p6 PROGMEM = 0b0000000000000000;
const word rim_p6 PROGMEM = 0b0000000000000000;
const word shaker_p6 PROGMEM = 0b0000000000000000;
const word cymbal_p6 PROGMEM = 0b0000000000000000;
const word kick_p6 PROGMEM = 0b0000000000000000;
const word snare_p6 PROGMEM = 0b0000000000000000;
const word clap_p6 PROGMEM = 0b0000000000000000;
const word cowbell_p6 PROGMEM = 0b0000000000000000;
const word claves_p6 PROGMEM = 0b0000000000000000;
const word congahigh_p6 PROGMEM = 0b0000000000000000;
const word congamid_p6 PROGMEM = 0b0000000000000000;
const word congalow_p6 PROGMEM = 0b0000000000000000;
const word pattern6[] PROGMEM = {hatclosed_p6, hatopen_p6, rim_p6, shaker_p6, cymbal_p6, kick_p6, snare_p6, clap_p6, cowbell_p6, claves_p6, congahigh_p6, congamid_p6, congalow_p6};

const word hatclosed_p7 PROGMEM = 0b0000000000000000;
const word hatopen_p7 PROGMEM = 0b0000000000000000;
const word rim_p7 PROGMEM = 0b0000000000000000;
const word shaker_p7 PROGMEM = 0b0000000000000000;
const word cymbal_p7 PROGMEM = 0b0000000000000000;
const word kick_p7 PROGMEM = 0b0000000000000000;
const word snare_p7 PROGMEM = 0b0000000000000000;
const word clap_p7 PROGMEM = 0b0000000000000000;
const word cowbell_p7 PROGMEM = 0b0000000000000000;
const word claves_p7 PROGMEM = 0b0000000000000000;
const word congahigh_p7 PROGMEM = 0b0000000000000000;
const word congamid_p7 PROGMEM = 0b0000000000000000;
const word congalow_p7 PROGMEM = 0b0000000000000000;
const word pattern7[] PROGMEM = {hatclosed_p7, hatopen_p7, rim_p7, shaker_p7, cymbal_p7, kick_p7, snare_p7, clap_p7, cowbell_p7, claves_p7, congahigh_p7, congamid_p7, congalow_p7};

const word hatclosed_p8 PROGMEM = 0b0000000000000000;
const word hatopen_p8 PROGMEM = 0b0000000000000000;
const word rim_p8 PROGMEM = 0b0000000000000000;
const word shaker_p8 PROGMEM = 0b0000000000000000;
const word cymbal_p8 PROGMEM = 0b0000000000000000;
const word kick_p8 PROGMEM = 0b0000000000000000;
const word snare_p8 PROGMEM = 0b0000000000000000;
const word clap_p8 PROGMEM = 0b0000000000000000;
const word cowbell_p8 PROGMEM = 0b0000000000000000;
const word claves_p8 PROGMEM = 0b0000000000000000;
const word congahigh_p8 PROGMEM = 0b0000000000000000;
const word congamid_p8 PROGMEM = 0b0000000000000000;
const word congalow_p8 PROGMEM = 0b0000000000000000;
const word pattern8[] PROGMEM = {hatclosed_p8, hatopen_p8, rim_p8, shaker_p8, cymbal_p8, kick_p8, snare_p8, clap_p8, cowbell_p8, claves_p8, congahigh_p8, congamid_p8, congalow_p8};

const word hatclosed_p9 PROGMEM = 0b0000000000000000;
const word hatopen_p9 PROGMEM = 0b0000000000000000;
const word rim_p9 PROGMEM = 0b0000000000000000;
const word shaker_p9 PROGMEM = 0b0000000000000000;
const word cymbal_p9 PROGMEM = 0b0000000000000000;
const word kick_p9 PROGMEM = 0b0000000000000000;
const word snare_p9 PROGMEM = 0b0000000000000000;
const word clap_p9 PROGMEM = 0b0000000000000000;
const word cowbell_p9 PROGMEM = 0b0000000000000000;
const word claves_p9 PROGMEM = 0b0000000000000000;
const word congahigh_p9 PROGMEM = 0b0000000000000000;
const word congamid_p9 PROGMEM = 0b0000000000000000;
const word congalow_p9 PROGMEM = 0b0000000000000000;
const word pattern9[] PROGMEM = {hatclosed_p9, hatopen_p9, rim_p9, shaker_p9, cymbal_p9, kick_p9, snare_p9, clap_p9, cowbell_p9, claves_p9, congahigh_p9, congamid_p9, congalow_p9};

const word hatclosed_p10 PROGMEM = 0b0000000000000000;
const word hatopen_p10 PROGMEM = 0b0000000000000000;
const word rim_p10 PROGMEM = 0b0000000000000000;
const word shaker_p10 PROGMEM = 0b0000000000000000;
const word cymbal_p10 PROGMEM = 0b0000000000000000;
const word kick_p10 PROGMEM = 0b0000000000000000;
const word snare_p10 PROGMEM = 0b0000000000000000;
const word clap_p10 PROGMEM = 0b0000000000000000;
const word cowbell_p10 PROGMEM = 0b0000000000000000;
const word claves_p10 PROGMEM = 0b0000000000000000;
const word congahigh_p10 PROGMEM = 0b0000000000000000;
const word congamid_p10 PROGMEM = 0b0000000000000000;
const word congalow_p10 PROGMEM = 0b0000000000000000;
const word pattern10[] PROGMEM = {hatclosed_p10, hatopen_p10, rim_p10, shaker_p10, cymbal_p10, kick_p10, snare_p10, clap_p10, cowbell_p10, claves_p10, congahigh_p10, congamid_p10, congalow_p10};

const word hatclosed_p11 PROGMEM = 0b0000000000000000;
const word hatopen_p11 PROGMEM = 0b0000000000000000;
const word rim_p11 PROGMEM = 0b0000000000000000;
const word shaker_p11 PROGMEM = 0b0000000000000000;
const word cymbal_p11 PROGMEM = 0b0000000000000000;
const word kick_p11 PROGMEM = 0b0000000000000000;
const word snare_p11 PROGMEM = 0b0000000000000000;
const word clap_p11 PROGMEM = 0b0000000000000000;
const word cowbell_p11 PROGMEM = 0b0000000000000000;
const word claves_p11 PROGMEM = 0b0000000000000000;
const word congahigh_p11 PROGMEM = 0b0000000000000000;
const word congamid_p11 PROGMEM = 0b0000000000000000;
const word congalow_p11 PROGMEM = 0b0000000000000000;
const word pattern11[] PROGMEM = {hatclosed_p11, hatopen_p11, rim_p11, shaker_p11, cymbal_p11, kick_p11, snare_p11, clap_p11, cowbell_p11, claves_p11, congahigh_p11, congamid_p11, congalow_p11};

const word hatclosed_p12 PROGMEM = 0b0000000000000000;
const word hatopen_p12 PROGMEM = 0b0000000000000000;
const word rim_p12 PROGMEM = 0b0000000000000000;
const word shaker_p12 PROGMEM = 0b0000000000000000;
const word cymbal_p12 PROGMEM = 0b0000000000000000;
const word kick_p12 PROGMEM = 0b0000000000000000;
const word snare_p12 PROGMEM = 0b0000000000000000;
const word clap_p12 PROGMEM = 0b0000000000000000;
const word cowbell_p12 PROGMEM = 0b0000000000000000;
const word claves_p12 PROGMEM = 0b0000000000000000;
const word congahigh_p12 PROGMEM = 0b0000000000000000;
const word congamid_p12 PROGMEM = 0b0000000000000000;
const word congalow_p12 PROGMEM = 0b0000000000000000;
const word pattern12[] PROGMEM = {hatclosed_p12, hatopen_p12, rim_p12, shaker_p12, cymbal_p12, kick_p12, snare_p12, clap_p12, cowbell_p12, claves_p12, congahigh_p12, congamid_p12, congalow_p12};

const word hatclosed_p13 PROGMEM = 0b0000000000000000;
const word hatopen_p13 PROGMEM = 0b0000000000000000;
const word rim_p13 PROGMEM = 0b0000000000000000;
const word shaker_p13 PROGMEM = 0b0000000000000000;
const word cymbal_p13 PROGMEM = 0b0000000000000000;
const word kick_p13 PROGMEM = 0b0000000000000000;
const word snare_p13 PROGMEM = 0b0000000000000000;
const word clap_p13 PROGMEM = 0b0000000000000000;
const word cowbell_p13 PROGMEM = 0b0000000000000000;
const word claves_p13 PROGMEM = 0b0000000000000000;
const word congahigh_p13 PROGMEM = 0b0000000000000000;
const word congamid_p13 PROGMEM = 0b0000000000000000;
const word congalow_p13 PROGMEM = 0b0000000000000000;
const word pattern13[] PROGMEM = {hatclosed_p13, hatopen_p13, rim_p13, shaker_p13, cymbal_p13, kick_p13, snare_p13, clap_p13, cowbell_p13, claves_p13, congahigh_p13, congamid_p13, congalow_p13};

const word hatclosed_p14 PROGMEM = 0b0000000000000000;
const word hatopen_p14 PROGMEM = 0b0000000000000000;
const word rim_p14 PROGMEM = 0b0000000000000000;
const word shaker_p14 PROGMEM = 0b0000000000000000;
const word cymbal_p14 PROGMEM = 0b0000000000000000;
const word kick_p14 PROGMEM = 0b0000000000000000;
const word snare_p14 PROGMEM = 0b0000000000000000;
const word clap_p14 PROGMEM = 0b0000000000000000;
const word cowbell_p14 PROGMEM = 0b0000000000000000;
const word claves_p14 PROGMEM = 0b0000000000000000;
const word congahigh_p14 PROGMEM = 0b0000000000000000;
const word congamid_p14 PROGMEM = 0b0000000000000000;
const word congalow_p14 PROGMEM = 0b0000000000000000;
const word pattern14[] PROGMEM = {hatclosed_p14, hatopen_p14, rim_p14, shaker_p14, cymbal_p14, kick_p14, snare_p14, clap_p14, cowbell_p14, claves_p14, congahigh_p14, congamid_p14, congalow_p14};

const word hatclosed_p15 PROGMEM = 0b0000000000000000;
const word hatopen_p15 PROGMEM = 0b0000000000000000;
const word rim_p15 PROGMEM = 0b0000000000000000;
const word shaker_p15 PROGMEM = 0b0000000000000000;
const word cymbal_p15 PROGMEM = 0b0000000000000000;
const word kick_p15 PROGMEM = 0b0000000000000000;
const word snare_p15 PROGMEM = 0b0000000000000000;
const word clap_p15 PROGMEM = 0b0000000000000000;
const word cowbell_p15 PROGMEM = 0b0000000000000000;
const word claves_p15 PROGMEM = 0b0000000000000000;
const word congahigh_p15 PROGMEM = 0b0000000000000000;
const word congamid_p15 PROGMEM = 0b0000000000000000;
const word congalow_p15 PROGMEM = 0b0000000000000000;
const word pattern15[] PROGMEM = {hatclosed_p15, hatopen_p15, rim_p15, shaker_p15, cymbal_p15, kick_p15, snare_p15, clap_p15, cowbell_p15, claves_p15, congahigh_p15, congamid_p15, congalow_p15};

const word hatclosed_p16 PROGMEM = 0b0000000000000000;
const word hatopen_p16 PROGMEM = 0b0000000000000000;
const word rim_p16 PROGMEM = 0b0000000000000000;
const word shaker_p16 PROGMEM = 0b0000000000000000;
const word cymbal_p16 PROGMEM = 0b0000000000000000;
const word kick_p16 PROGMEM = 0b0000000000000000;
const word snare_p16 PROGMEM = 0b0000000000000000;
const word clap_p16 PROGMEM = 0b0000000000000000;
const word cowbell_p16 PROGMEM = 0b0000000000000000;
const word claves_p16 PROGMEM = 0b0000000000000000;
const word congahigh_p16 PROGMEM = 0b0000000000000000;
const word congamid_p16 PROGMEM = 0b0000000000000000;
const word congalow_p16 PROGMEM = 0b0000000000000000;
const word pattern16[] PROGMEM = {hatclosed_p16, hatopen_p16, rim_p16, shaker_p16, cymbal_p16, kick_p16, snare_p16, clap_p16, cowbell_p16, claves_p16, congahigh_p16, congamid_p16, congalow_p16};

// this class contains all the information contained in 1 Drummer Boy bar of music excluding sample volumes which are only stored/restored when saving/loading
class ActivePattern
{
  public:
    word hatclosed = 0b0000000000000000;
    word hatopen = 0b0000000000000000;
    word rim = 0b0000000000000000;
    word shaker = 0b0000000000000000;
    word cymbal = 0b0000000000000000;
    
    word kick = 0b0000000000000000;
    word snare = 0b0000000000000000;
    word clap = 0b0000000000000000;
    word cowbell = 0b0000000000000000;

    word claves = 0b0000000000000000;
    word congahigh = 0b0000000000000000;
    word congamid = 0b0000000000000000;
    word congalow = 0b0000000000000000;
};

ActivePattern activePattern;

// ------------------------------------------------------------------  SAMPLES ------------------------------------------------------------//

class Samples
{
  public:
    word hatclosed = 0;
    word hatopen = 0;
    word rim = 0;
    word shaker = 0;
    word cymbal = 0;
    
    word kick = 0;
    word snare = 0;
    word clap = 0;
    word cowbell = 0;

    word claves = 0;
    word congahigh = 0;
    word congamid = 0;
    word congalow = 0;
};

Samples activeSamples;
Samples samples_808;
Samples samples_mixed;



// ------------------------------------------------------------------    DECLARE PINS AND CONSTANTS ------------------------------------------------------------//
const int tempoPin = A0;
const int volumePin = A1;

const int pauseSwitchPin = 2;
const int swingSwitchPin = 3;
const int fillButtonPin = 4;

//--------------------------------------------------------------VARIABLE DECLARATION & INITIALIZATION------------------------------------------------------//
boolean swingOn = false;
boolean fillOn = false;
boolean pauseOn = false;

int tempoMS;
int volume;
int swingMS = 0;
int tempoBPM;

word beatMaster = 0;

//------------------------------------------------------------------- FUNCTION PROTOTYPES--------------------------------------------------------------------------------------------------//

// FUNCTION PROTOTYPES // -  The Arduino IDE creates function prototypes for you. Normally, this works quite well. There are some situations, like functions with reference arguments, where it doesn't.
void checkAnalogIO();
void checkDigitalIO();
void fill();


// ***************************************SETUP*******************************************************
void setup() {
  
  // SET 808 SAMPLES
  samples_808.hatclosed = 1;
  samples_808.hatopen = 2;
  samples_808.rim = 3;
  samples_808.shaker = 4;
  samples_808.cymbal = 5;
  
  samples_808.kick = 6;
  samples_808.snare = 7;
  samples_808.clap = 8;
  samples_808.cowbell = 9;
  
  samples_808.claves = 10;
  samples_808.congahigh = 11;
  samples_808.congamid = 12;
  samples_808.congalow = 13;

    // MIXED SAMPLES SET
  samples_mixed.hatclosed = 17;
  samples_mixed.hatopen = 18;
  samples_mixed.rim = 26;
  samples_mixed.shaker = 28;
  samples_mixed.cymbal = 29;
  
  samples_mixed.kick = 19;
  samples_mixed.snare = 22;
  samples_mixed.clap = 30;
  samples_mixed.cowbell = 27;
  
  samples_mixed.claves = 21;
  samples_mixed.congahigh = 23;
  samples_mixed.congamid = 24;
  samples_mixed.congalow = 25;



  
  // Serial monitor for debugging
  Serial.begin(9600);

  Serial.println("hello world");

  delay(100);

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
//  pinMode(fillButtonPin, INPUT_PULLUP);
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
    
    if (fillOn) fill();

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

void switchSamples(int s){
  if(s == 1) transferSamples(samples_808);
  if(s == 2) transferSamples(samples_mixed);
  if(s == 3) transferSamples(samples_808);
}

void transferSamples(Samples &passedSamples){
  activeSamples = passedSamples;
}

void switchPattern(int p) {
  if(p == 1) transferPatternMem(pattern1);
  if(p == 2) transferPatternMem(pattern2);
  if(p == 3) transferPatternMem(pattern3);
  if(p == 4) transferPatternMem(pattern4);

  if(p == 5) transferPatternMem(pattern5);
  if(p == 6) transferPatternMem(pattern6);
  if(p == 7) transferPatternMem(pattern7);
  if(p == 8) transferPatternMem(pattern8);

  if(p == 9) transferPatternMem(pattern9);
  if(p == 10) transferPatternMem(pattern10);
  if(p == 11) transferPatternMem(pattern11);
  if(p == 12) transferPatternMem(pattern12);

  if(p == 13) transferPatternMem(pattern13);
  if(p == 14) transferPatternMem(pattern14);
  if(p == 15) transferPatternMem(pattern15);
  if(p == 16) transferPatternMem(pattern16);
}

void transferPatternMem(const word memPattern[]){
  activePattern.hatclosed = pgm_read_word( &memPattern[0] );
  activePattern.hatopen = pgm_read_word( &memPattern[1] );
  activePattern.rim = pgm_read_word( &memPattern[2] );
  activePattern.shaker = pgm_read_word( &memPattern[3] );
  activePattern.cymbal = pgm_read_word( &memPattern[4] );

  activePattern.kick = pgm_read_word( &memPattern[5] );
  activePattern.snare = pgm_read_word( &memPattern[6] );
  activePattern.clap = pgm_read_word( &memPattern[7] );
  activePattern.cowbell = pgm_read_word( &memPattern[8] );

  activePattern.claves = pgm_read_word( &memPattern[9] );
  activePattern.congahigh = pgm_read_word( &memPattern[10] );
  activePattern.congamid = pgm_read_word( &memPattern[11] );
  activePattern.congalow = pgm_read_word( &memPattern[12] );
}

void fill() {
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
    readFillButton();
  }
}

void readFillButton() {
  if ( digitalRead(fillButtonPin) == LOW && !fillOn) {
    fillOn = true;
    Serial.println("Fill ON");
  }
  if (digitalRead(fillButtonPin) == HIGH && fillOn && beatMaster == 15) {
    fillOn = false;
    Serial.println("Fill OFF");
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
