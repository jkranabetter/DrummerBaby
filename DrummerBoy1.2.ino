#include <Metro.h>
#include <wavTrigger.h>


Metro seqMetro(1000);        // Sequencer state machine interval timer
Metro tripletMetro(1000);
Metro readAnalogInputs(100);      // Read potentiometer values
Metro readDigitalInputsHP(50);     //
Metro readDigitalInputsLP(150);



//WAV Trigger will only play WAV files formatted as 16-bit, stereo, 44.1kHz, and there can be no meta-data (non-audio data) in the file before the audio data.
//Make sure the file name starts with a 3 digit number.
wavTrigger wTrig;             // Our WAV Trigger object


// ------------------------------------------------------------------    DECLARE PINS AND CONSTANTS ------------------------------------------------------------//
// const avoids the pitfalls of the define operator (essentially search and replace)


const int sampleRatePin = A0; // Analog input pins
const int swingPin = A1;
const int trckVolPn = A2;
const int tempoPin = A3;
const int volumePin = A4;


const int stutterSwitchPin = 19;
const int swingSwitchPin = 17;
const int pauseSwitchPin = 43;


//--------------------------------------------------------------VARIABLE DECLARATION & INITIALIZATION------------------------------------------------------//

// arrays containing sample wav numbers
// since we use byte for storage the sample numbers are limited to 0-255 range, this is easily changed but uses more dynamic memory (sram)
const byte hat[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
const byte snare[] = {17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32};
const byte kick[] = {33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48};
const byte perc1[] = {49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64};
const byte perc2[] = {65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80};
const byte sample1[] = {81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96};
const byte sample2[] = {97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112};
const byte hit[] = {113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128};
const byte livePlaySamples[16] = {250, 251, 11, 15, 21, 27, 31, 33, 35, 37, 38, 43, 44, 77, 78, 79};


//bool & ints
boolean clockOutLogicHigh = false;
boolean beatBtnPshdRN = 0;
boolean accentBtnPshdRN = 0;
boolean polyBtnPshdRN = 0;
boolean rollBtnPshdRN = 0;
boolean groupSelectPushedRN = 0;
boolean tapBtnPushedRN = 0;
boolean sampleSelectPushedRN = 0;
boolean erasePushedRN = 0;
boolean accentPushedRN = 0;
boolean functionBtnPushedRN = 0;

boolean trackVolBtnPushedRN = 0;
boolean polyBtnPushedRN = 0;
boolean accent = 0;
boolean poly = 0;
word currentGroup = 0;
word lastGroup = 1;
word currentSlotPtr = 0;
word tempStorage = 0;
word lastBtnPushed;
word lastAccentBtnPushed;
word lastRollBtnPushed;
word lastPolyBtnPushed;

static int tempoMS;
static int tripletMS;
static int volume;
word beatMaster = 0;
byte beatMasterTriplet = 0;
word beatHat = 0;  // set initial beat locations
word beatSnare = 0;
word beatKick = 0;
word beatPerc1 = 0;
word beatPerc2 = 0;
word beatSmp1 = 0;
word beatHit = 0;
word beatSmp2 = 0;
word prevBeatHat = 15;
word prevBeatSnare  = 15;
word prevBeatKick = 15;
word prevBeatPerc1 = 15;
word prevBeatPerc2 = 15;
word prevBeatSmp1 = 15;
word prevBeatHit = 15;
word prevBeatSmp2 = 15;
word lastHatBeat = 16;
word lastSnareBeat = 16;
word lastKickBeat = 16;
word lastBassBeat = 16;
word lastPerc1Beat = 16;
word lastPerc2Beat = 16;
word lastSmp1Beat = 16;
word lastHitBeat = 16;
word lastSmp2Beat = 16;
word selectedLastBeat = 0;
word accentDBOffset = 20;      // default 20
int startTime;   // for "tap" entry timing
int startTimeTriplet;
long lastNormalRollMs = 0;
long lastTripletRollMs = 0;
long lastClockMs = 0;
word rollCounterNormal = 0;
word rollCounterTriplet = 0;
int reading;
int halfMS;
int halfTripletMS;
//led display variables
byte data;
byte data2;

boolean groupLeds[8];
boolean displayingLiveMash = false;
int tempoBPM;
char saveName[16];
int accentVolLastIntermediate;

boolean swingOn = false;
boolean stutterOn = false;
boolean pauseOn = false;
int swingMS = 0;
boolean synced = false;
boolean fastSynced = false;

char trackVolumes[NUM_TRACKS] = {0}; // char data type range is -128 to 127 which is perfect for out volume range of -70 to 10

int delayTime = 40;
boolean delayFlag = 0;
boolean delayTripletFlag = 0;
long startMS_d = 0;
long startMS_tripD = 0;
word lastObj = 0;
word lastPatt = 0; // holds last pattern that was printed to lcd



//----------------------------------------------------------------------------------OBJECT CREATION-----------------------------------------------------------------------------//
// this class contains all the information contained in 1 Drummer Boy bar of music excluding sample volumes which are only stored/restored when saving/loading

class Bar
{
  public:
    byte hatActiveSamples[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    byte snareActiveSamples[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    byte kickActiveSamples[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    byte perc1ActiveSamples[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    byte perc2ActiveSamples[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    byte smp1ActiveSamples[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    byte smp2ActiveSamples[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    byte hitActiveSamples[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    byte rollSamples[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    boolean tripletGroup[8] = {false,  false, false, false, false, false, false, false };
    boolean shuffleGroup[8] = {false,  false, false, false, false, false, false, false };

    word hatAccents;
    word snareAccents;
    word kickAccents;
    word perc1Accents;
    word perc2Accents;
    word smpAccents;
    word smp2Accents;
    word hitAccents;

    word buttonsHat;
    word buttonsSnare;
    word buttonsKick;
    word buttonsPerc1;
    word buttonsPerc2;
    word buttonsSmp1;
    word buttonsSmp2;
    word buttonsHit;

};

// 192 bytes each
Bar a;
Bar b;
Bar c;
Bar f;

struct saveLoad     // declare a structure for saving/loading
{
  Bar a_ROM;
  Bar b_ROM;
  Bar c_ROM;
  Bar f_ROM;
  char trackVolumes_ROM[NUM_TRACKS] = {0};
  byte chain_ROM[4] = {0};  //modifymem
} memory;

//------------------------------------------------------------------- FUNCTION PROTOTYPES--------------------------------------------------------------------------------------------------//

// FUNCTION PROTOTYPES // -  The Arduino IDE creates function prototypes for you. Normally, this works quite well. There are some situations, like functions with reference arguments, where it doesn't.
void setAccents(Bar &obj, boolean triplet);
void setAccent(int i, int nextBeat, word accentArray, Bar &obj);
void checkAnalogIO(Bar &obj);
void loadNextBeat(Bar &obj, boolean triplet);
void checkAccentButtons(Bar &obj);
void checkDigitalIO(Bar &obj);
void checkMedPriorityButtons(Bar &obj);
void save(Bar &obj);
void load(Bar &obj);
void checkLowPriorityButtons(Bar &obj);
void changeActiveSamples(int last, int current, Bar &obj);
void copyToActiveGroup(Bar &obj);
void copyTFromActiveGroup(Bar &obj);
void addSample(int beat, Bar &obj);
void checkBeatButtons(Bar &obj);
void checkHighPriorityButtons(Bar &obj);
void switchGroup(Bar &obj);
void removeSample(int beat, Bar &obj);
void readTrackVolume(boolean firstCall, Bar &obj);
void readGroup(boolean firstCall, Bar &obj);
void setTrackVolumes();
void readBtn_Sample(Bar &obj);
void readBtn_Erase(Bar &obj);
void readBtn_Accent(Bar &obj);
void readBtn_TrackVol(Bar &obj);
void readBtn_Poly(Bar &obj);
void readBtn_Tap(Bar &obj);
void readBtn_Function(Bar &obj);
int getSelection(Bar & obj, boolean saving);
void playRoll(Bar &obj);
void addRollSample(int beat, Bar &obj);
void removeRollSample(int beat, Bar &obj);
void incrementBeatCounters(boolean triplet, Bar &obj);
void setTriplet(Bar &obj);
boolean tripletActiveforCrntGrp(Bar &obj);
void waitForDelay(Bar &obj);
void checkPattern(Bar &obj);
Bar getObjectByRef(int ptr);
void mainLoop(Bar &object);
Bar getOb();
void randomize(Bar &obj);
void randomizeContinuously(Bar &obj);
void shuffle(Bar &obj);
void stutter(Bar &obj);
void playNormalRoll(Bar &obj, byte beat);
void playTripletRoll(Bar &obj, byte beat);
//void updateLCD(boolean updateSampleName, int updateBpm, int);



// ***************************************SETUP*******************************************************
void setup() {
  // set default track volumes to prevent anvil/stirrup combustion
  trackVolumes[4] = -4; //ice hat
  trackVolumes[9] = -4; //click
  trackVolumes[11] = -4; //open hat
  trackVolumes[12] = -20; //grav crash
  trackVolumes[15] = -10; //chiptune hh open
  trackVolumes[16] = -5; //chiptune hh closed
  trackVolumes[29] = -5; //chiptune snare 1
  trackVolumes[30] = -5; //chiptune snare 2
  trackVolumes[31] = -5; //chiptune snare 3
  trackVolumes[33] = -5; //808 kick
  trackVolumes[35] = -8; //MAL kick
  trackVolumes[37] = -8; //LINN kick
  trackVolumes[38] = -10; //House kick
  trackVolumes[40] = -10; //House gen
  trackVolumes[57] = -5; //bloop
  trackVolumes[58] = -15; //triangle
  trackVolumes[59] = -15; //glassbell
  trackVolumes[64] = -15; //909 clav
  trackVolumes[67] = -15; //phone 1
  trackVolumes[67] = -5; //phone 3
  trackVolumes[81] = -10; //cha1
  trackVolumes[86] = -15; //echo harp
  trackVolumes[87] = -10; //punch
  trackVolumes[88] = -10; //chicka 1
  trackVolumes[94] = -15; //laser
  trackVolumes[95] = -10; //noisefall
  trackVolumes[96] = -15; //let me hear you say
  trackVolumes[98] = -5; //gun
  trackVolumes[99] = -20; //sound of wealth
  trackVolumes[100] = -5; //oh
  trackVolumes[101] = -10; //going up
  trackVolumes[113] = -12; //orchestral hit
  trackVolumes[116] = -12; //dancemachine
  trackVolumes[117] = -12; //dancemachine
  trackVolumes[118] = -5; //yea
  trackVolumes[119] = -12; //sword
  trackVolumes[120] = -12; //get ready
  trackVolumes[121] = -12; //church bell

  // create objects, each is 1 bar (16 beats)
  a = Bar();
  b = Bar();
  c = Bar();
  f = Bar();

  //for eeprom SDA SCL (I2C communication)
  Wire.begin();
  // Serial monitor for debugging
  Serial.begin(9600);

  pinMode(latchPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockOutPin, OUTPUT);

  pinMode(buttonPin[0], INPUT_PULLUP);
  pinMode(buttonPin[1], INPUT_PULLUP);
  pinMode(buttonPin[2], INPUT_PULLUP);
  pinMode(buttonPin[3], INPUT_PULLUP);
  pinMode(buttonPin[4], INPUT_PULLUP);
  pinMode(buttonPin[5], INPUT_PULLUP);
  pinMode(buttonPin[6], INPUT_PULLUP);
  pinMode(buttonPin[7], INPUT_PULLUP);
  pinMode(buttonPin[8], INPUT_PULLUP);
  pinMode(buttonPin[9], INPUT_PULLUP);
  pinMode(buttonPin[10], INPUT_PULLUP);
  pinMode(buttonPin[11], INPUT_PULLUP);
  pinMode(buttonPin[12], INPUT_PULLUP);
  pinMode(buttonPin[13], INPUT_PULLUP);
  pinMode(buttonPin[14], INPUT_PULLUP);
  pinMode(buttonPin[15], INPUT_PULLUP);
  pinMode(eraseBtnPin, INPUT_PULLUP);
  pinMode(functionBtnPin, INPUT_PULLUP);
  pinMode(accentSelectPin, INPUT_PULLUP);
  pinMode(polyBtnPin, INPUT_PULLUP);
  pinMode(tapBtnPin, INPUT_PULLUP);
  pinMode(trackVolBtnPin, INPUT_PULLUP);

  pinMode(sampleTogglePin, INPUT_PULLUP);

  pinMode(pitchSwitchPin, INPUT_PULLUP);
  pinMode(swingSwitchPin, INPUT_PULLUP);
  pinMode(stutterSwitchPin, INPUT_PULLUP);
  pinMode(pauseSwitchPin, INPUT_PULLUP);
  pinMode(gSwitchPin[0], INPUT_PULLUP);
  pinMode(gSwitchPin[1], INPUT_PULLUP);
  pinMode(gSwitchPin[2], INPUT_PULLUP);
  pinMode(gSwitchPin[3], INPUT_PULLUP);
  pinMode(gSwitchPin[4], INPUT_PULLUP);
  pinMode(gSwitchPin[5], INPUT_PULLUP);
  pinMode(gSwitchPin[6], INPUT_PULLUP);
  pinMode(gSwitchPin[7], INPUT_PULLUP);

  delay(500); // wait for the WAV Trigger to finish reset before trying to send commands.
  wTrig.start(); // WAV Trigger startup at 57600
  delay(10);
  wTrig.stopAllTracks();// Send a stop-all command and reset the sample-rate offset, in case we have reset while the WAV Trigger was already playing.
  wTrig.samplerateOffset(0); // make sure the pitch is unmodified


  // enable track reporting (recieve data from the WAV Trigger)
  wTrig.setReporting(true);
  delay(100);
  // get version/track info from the wav trigger
  char gWTrigVersion[VERSION_STRING_LEN];    // WAV Trigger version string
  if (wTrig.getVersion(gWTrigVersion, VERSION_STRING_LEN)) {
    Serial.print(gWTrigVersion);
    Serial.print("\n");
    int gNumTracks = wTrig.getNumTracks();
    Serial.print("Number of tracks = ");
    Serial.println(gNumTracks);
  }
  else
    Serial.print("WAV Trigger response not available");

  delay(1000);
  updateLCD(true, 0, true);  // update all info on the lcd

  readTempo(1); // read vol and tempo values
  readVolume(1);
  wTrig.trackPlayPoly(200);   // play startup sound

}

//------------------------------------------------------------------------------------MAIN LOOP------------------------------------------------------------------------//


void loop() {
  if (chain[currentSlotPtr] == 1) mainLoop(a);
  if (chain[currentSlotPtr] == 2) mainLoop(b);
  if (chain[currentSlotPtr] == 3) mainLoop(c);
  if (chain[currentSlotPtr] == 4) mainLoop(f);
}


void mainLoop(Bar &object) {
  if (lastObj != chain[currentSlotPtr]) {
    copyToActiveGroup(object);
    lastObj = chain[currentSlotPtr];
  }
  if (!pauseOn) {
    if (seqMetro.check() == 1) {
      //Serial.println("quarter");
      setSwing();
      setAccents(object, false); //set accents before to prevent change in volume while track is playing
      lastNormalRollMs = millis(); // for roll
      rollCounterNormal = 0; // for roll
      for (int i = 0; i < 8 ; i++) {
        if ( playSamplesBuffer[i] != 0) {
          if (groupSwitchState[i] == 1 && !object.tripletGroup[i]) {
            if (!delayGroup[i]) {
              wTrig.trackPlayPoly( playSamplesBuffer[i] );
              //Serial.print("Playing sample:"); Serial.println(playSamplesBuffer[i]);
            }
            else {
              playSamplesDelayBuffer[i] = playSamplesBuffer[i];
              startMS_d = millis();
              delayFlag = true;
              Serial.print("liveMash:"); Serial.println(liveMash);
            }
          }
        }
      }
      if (delayFlag == false) {
        incrementBeatCounters(false, object);
        if (beatMaster == 14)shuffle(object);
        if (beatMaster == 15) checkPattern(object);
        loadNextBeat(object, false);
      }
      startTime = millis();
    }
    
    waitForDelay(object);
    playRoll(object);
    checkDigitalIO(object);
    checkAnalogIO(object);
    clockOut();
    if (stutterOn) stutter(object);
  } else {
    if (digitalRead(pauseSwitchPin) == HIGH && pauseOn) {
      pauseOn = false;
      seqMetro.reset();
      tripletMetro.reset();
      startAtBeat0();
    }
    checkDigitalIO(object);
    checkAnalogIO(object);
  }
}

////////////////////////////////////////////////////////////////////////////////////// OOP Methods /////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void stutter(Bar &obj) {
  if ( !obj.tripletGroup[0]) beatHat = 15;
  else beatHat = 14;
  if ( !obj.tripletGroup[1]) beatSnare = 15;
  else beatSnare = 14;
  if ( !obj.tripletGroup[2]) beatKick = 15;
  else beatKick = 14;
  if ( !obj.tripletGroup[3]) beatPerc1 = 15;
  else beatPerc1 = 14;
  if ( !obj.tripletGroup[4]) beatPerc2 = 15;
  else beatPerc2 = 14;
  if ( !obj.tripletGroup[5]) beatSmp1 = 15;
  else beatSmp1 = 14;
  if ( !obj.tripletGroup[6]) beatSmp2 = 15;
  else beatSmp2 = 14;
  if ( !obj.tripletGroup[7]) beatHit = 15;
  else beatHit = 14;
}

//------------------------------------------------------------------------------------LOOP METHODS------------------------------------------------------------------------//


void startAtBeat0() {
  beatHat = 0;
  beatSnare = 0;
  beatKick = 0;
  beatPerc1 = 0;
  beatPerc2 = 0;
  beatSmp1 = 0;
  beatSmp2 = 0;
  beatHit = 0;
  beatMaster = 0;
  beatMasterTriplet = 0;

  prevBeatHat = 15;
  prevBeatSnare  = 15;
  prevBeatKick = 15;
  prevBeatPerc1 = 15;
  prevBeatPerc2 = 15;
  prevBeatSmp1  = 15;
  prevBeatSmp2 = 15;
  prevBeatHit = 15;
}

void incrementBeatCounters(boolean triplet, Bar &obj) {
  if (!triplet) beatMaster++;
  if (triplet) beatMasterTriplet++;

  if (triplet == obj.tripletGroup[0]) { //increments for triplets and 4/4
    prevBeatHat = beatHat;    // keep track of the previous beat
    beatHat++;
  }
  if (triplet == obj.tripletGroup[1]) {
    prevBeatSnare = beatSnare;
    beatSnare++;
  }
  if (triplet == obj.tripletGroup[2]) {
    prevBeatKick = beatKick;
    beatKick++;
  }
  if (triplet == obj.tripletGroup[3]) {
    prevBeatPerc1 = beatPerc1;
    beatPerc1++;
  }
  if (triplet == obj.tripletGroup[4]) {
    prevBeatPerc2 = beatPerc2;
    beatPerc2++;
  }
  if (triplet == obj.tripletGroup[5]) {
    prevBeatSmp1 = beatSmp1;
    beatSmp1++;
  }
  if (triplet == obj.tripletGroup[6]) {
    prevBeatSmp2 = beatSmp2;
    beatSmp2++;
  }
  if (triplet == obj.tripletGroup[7]) {
    prevBeatHit = beatHit;
    beatHit++;
  }

  // sync the master and triplet beats once at the start of every measure, they are called usually a few ms apart in random order so wait until both have played their first beats
  if (beatMaster == 1 && beatMasterTriplet == 1) {
    if (!synced) {
      tripletMetro.previous_millis = seqMetro.previous_millis;    //sync timers
      synced = true;
    }
  }
  if (beatMaster == 2 && synced) {
    synced = false; //reset sync
  }
  //set a triplet group instantly after the first beat has been played
  //Serial.print("flag: "); Serial.print(tripletFlag);
  //Serial.print("  syncd: "); Serial.print(synced);
  //Serial.print("  beat: "); Serial.print(beatMaster);
  //Serial.print("  tbeat: "); Serial.println(beatMasterTriplet);
  if ( tripletFlag && synced ) {
    setTriplet(obj);
    Serial.println("setting triplet");
  }

  // skip every 4rth beat for triplet groups
  if (triplet) {
    if (beatMasterTriplet == 3 || beatMasterTriplet == 7 || beatMasterTriplet == 11 || beatMasterTriplet == 15) beatMasterTriplet++;
    if (obj.tripletGroup[0] && ( beatHat == 3 || beatHat == 7 || beatHat == 11 || beatHat == 15)) beatHat++;      //triplets use the same array but skip every fourth beat
    if (obj.tripletGroup[1] && ( beatSnare == 3 || beatSnare == 7 || beatSnare == 11 || beatSnare == 15)) beatSnare++;
    if (obj.tripletGroup[2] && ( beatKick == 3 || beatKick == 7 || beatKick == 11 || beatKick == 15)) beatKick++;
    if (obj.tripletGroup[3] && ( beatPerc1 == 3 || beatPerc1 == 7 || beatPerc1 == 11 || beatPerc1 == 15)) beatPerc1++;
    if (obj.tripletGroup[4] && (beatPerc2 == 3 || beatPerc2 == 7 || beatPerc2 == 11 || beatPerc2 == 15)) beatPerc2++;
    if (obj.tripletGroup[5] && (beatSmp1 == 3 || beatSmp1 == 7 || beatSmp1 == 11 || beatSmp1 == 15)) beatSmp1++;
    if (obj.tripletGroup[6] && (beatSmp2 == 3 || beatSmp2 == 7 || beatSmp2 == 11 || beatSmp2 == 15)) beatSmp2++;
    if (obj.tripletGroup[7] && (beatHit == 3 || beatHit == 7 || beatHit == 11 || beatHit == 15)) beatHit++;
  }
  if ( beatMaster >= 16 ) {  //reset master
    beatMaster = 0;
  }
  if ( beatMasterTriplet >= 16 ) {  //reset triplet master
    beatMasterTriplet = 0;
  }
  if ( beatHat >= lastHatBeat )   //reset beats depending on polyrythem
    beatHat = 0;
  if ( beatSnare >= lastSnareBeat )
    beatSnare = 0;
  if ( beatKick >= lastKickBeat )
    beatKick = 0;
  if ( beatPerc1 >= lastPerc1Beat )
    beatPerc1 = 0;
  if ( beatPerc2 >= lastPerc2Beat )
    beatPerc2 = 0;
  if ( beatSmp1 >= lastSmp1Beat )
    beatSmp1 = 0;
  if ( beatSmp2 >= lastSmp2Beat )
    beatSmp2 = 0;
  if ( beatHit >= lastHitBeat ) {
    beatHit = 0;
  }

  if (polyFlag && beatMaster == 1) {   //set polyrythem when master  has rolled over for resyncing
    setPoly();
  }
}

void loadNextBeat(Bar & obj, boolean triplet) {
  if (triplet == obj.tripletGroup[0]) playSamplesBuffer[0] = obj.hatActiveSamples[beatHat];
  if (triplet == obj.tripletGroup[1]) playSamplesBuffer[1] = obj.snareActiveSamples[beatSnare];
  if (triplet == obj.tripletGroup[2]) playSamplesBuffer[2] = obj.kickActiveSamples[beatKick];
  if (triplet == obj.tripletGroup[3]) playSamplesBuffer[3] = obj.perc1ActiveSamples[beatPerc1];
  if (triplet == obj.tripletGroup[4]) playSamplesBuffer[4] = obj.perc2ActiveSamples[beatPerc2];
  if (triplet == obj.tripletGroup[5]) playSamplesBuffer[5] = obj.smp1ActiveSamples[beatSmp1];
  if (triplet == obj.tripletGroup[6]) playSamplesBuffer[6] = obj.smp2ActiveSamples[beatSmp2];
  if (triplet == obj.tripletGroup[7]) playSamplesBuffer[7] = obj.hitActiveSamples[beatHit];
}

void setSwing() {
  if (swingOn) {
    if (beatMaster % 2 == 1)seqMetro.interval(tempoMS - swingMS);
    else seqMetro.interval(tempoMS + swingMS);
  }
}


//-iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii-IO SCAN-ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo-/
//---------------------- Analog -------------------//
void checkAnalogIO(Bar & obj) {
  if (readAnalogInputs.check() == 1) {  // if minumum time has passed the inputs are read
    readTempo(0);
    readVolume(0);
    readGroup(0, obj);
    if ( trackVolBtnPushedRN == 1 && !accent) readTrackVolume(0, obj);
    if (pitchOn == 1) readSampleRate(0);
    if (swingOn) readSwing(0);
    if(accent) readAccentVolume();
  }
}

void readSwing(boolean firstCall) {
  static int swingIntermediate;
  static int swingLastIntermediate;
  static int swingAmnt;
  //Serial.println("Checking swingAmnt");
  swingIntermediate = analogRead(swingPin);
  if ( abs( swingLastIntermediate - swingIntermediate) > 10 || firstCall) {
    swingLastIntermediate = swingIntermediate;
    swingAmnt = map(swingIntermediate, 0, 1023, 0, 51);  //scale to 0-50
    if (swingAmnt > 50) swingAmnt = 50;
    swingMS = swingAmnt * tempoMS / 100; // get 0% to 50% of MS between beats
    int percent = (tempoMS + swingMS) * 100 / (tempoMS * 2); // calculate percent based on Linns method - 50% is no swing, equal distribution between odd and even beats - top range is 75%
    //Serial.print("TempoMs: ");Serial.print(tempoMS);Serial.print("  Swing Rate: "); Serial.print(swingAmnt);Serial.print("   swingMS: ");Serial.print(swingMS);Serial.print("   Percent: ");Serial.println(percent);
  }
}

void readTrackVolume(boolean firstCall, Bar & obj) {      // all track volumes are initially set to 0
  // read analog input for volume and map to wav trigger vol range (-70 to +10)
  static int trackVolumeLastIntermediate;
  static int trackVolumeIntermediate;
  static int trackVolume;
  trackVolumeIntermediate = analogRead(trckVolPn);
  if ( abs( trackVolumeLastIntermediate - trackVolumeIntermediate) > 10 || firstCall) {
    trackVolumeLastIntermediate = trackVolumeIntermediate;
    trackVolume = map(trackVolumeIntermediate, 0, 1023, -30, 10);   // tracks can go to -70 but are pretty much inaudible by -30
    wTrig.trackGain(getCrrntSlctdSmpl(), trackVolume);
    trackVolumes[getCrrntSlctdSmpl()] = trackVolume;
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
    halfMS = tempoMS / 2;
    tripletMS = tempoMS * 4 / 3;
    halfTripletMS = tripletMS / 2;
    tripletMetro.interval(tripletMS);
    tempoBPM = 60000 / (tempoMS * 4);   /// ms to BPM conversion, the 4 is there because every DB "beat" is actually an eight note
    updateLCD(false, tempoBPM, false); // display new tempo on lcd, update only the BPM
  }
}

//---------------------- Digital -------------------//
void checkDigitalIO(Bar & obj) {
  if (readDigitalInputsHP.check() == 1) {
    if (accent && poly) {
      writeRollLEDs();
      checkRollButtons(obj);
    } else if (accent) {
      writeAccentLEDs();
      checkAccentButtons(obj);
    } else if (poly) {
      writePolyLEDs();
      checkPolyButtons();
    } else if (functionBtnPushedRN) {
      writePatternLEDs();
      checkBeatButtons(obj);
    } else {
      checkBeatButtons(obj);
      writeBeatLEDs();
    }
    checkHighPriorityButtons(obj);
    checkMedPriorityButtons(obj);
  }
}

void checkGroupSwitches() {
  for ( int j = 0 ; j < 8 ; j++) {
    if (digitalRead(gSwitchPin[j]) == HIGH && groupSwitchState[j]) {
      groupSwitchState[j] = 0;
    }
    if (digitalRead(gSwitchPin[j]) == LOW && !groupSwitchState[j]) {
      groupSwitchState[j] = 1;
    }
  }
}

void readSwingSwitch() {
  if ( digitalRead(swingSwitchPin) == LOW && !swingOn) {
    Serial.println("Swing ON");//
    readSwing(1);
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

void checkMedPriorityButtons(Bar & obj) {
  readBtn_Sample(obj);
  readBtn_Erase(obj);
  readBtn_Accent(obj);
  readBtn_TrackVol(obj);
  readBtn_Poly(obj);
}
