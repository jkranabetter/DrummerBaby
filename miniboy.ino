#include <Metro.h>
#include <wavTrigger.h>

Metro seqMetro(1000);        // Sequencer state machine interval timer
Metro tripletMetro(1000);
Metro readAnalogInputs(100);      // Read potentiometer values
Metro readDigitalInputsHP(50);     //


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
boolean swingOn = false;
boolean stutterOn = false;
boolean pauseOn = false;
boolean synced = false;
boolean fastSynced = false;

static int tempoMS;
static int tripletMS;
static int volume;
int swingMS = 0;
int tempoBPM;

word beatMaster = 0;


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

//------------------------------------------------------------------- FUNCTION PROTOTYPES--------------------------------------------------------------------------------------------------//

// FUNCTION PROTOTYPES // -  The Arduino IDE creates function prototypes for you. Normally, this works quite well. There are some situations, like functions with reference arguments, where it doesn't.
void checkAnalogIO(Bar &obj);
void checkDigitalIO(Bar &obj);
void stutter(Bar &obj);


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

  // Serial monitor for debugging
  Serial.begin(9600);

  pinMode(swingSwitchPin, INPUT_PULLUP);
  pinMode(stutterSwitchPin, INPUT_PULLUP);
  pinMode(pauseSwitchPin, INPUT_PULLUP);

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

  if (!pauseOn) {
    if (seqMetro.check() == 1) {
      setSwing();
      for (int i = 0; i < 8 ; i++) {
        wTrig.trackPlayPoly( );
      }
    }
    
    if (stutterOn) stutter(object);

  } else {
    if (digitalRead(pauseSwitchPin) == HIGH && pauseOn) {
      pauseOn = false;
      seqMetro.reset();
      startAtBeat0();
    }
    
  }
  checkDigitalIO(object);
  checkAnalogIO(object);
}

////////////////////////////////////////////////////////////////////////////////////// OOP Methods /////////////////////////////////////////////////////////////////////////////////////////////////////

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
  beatMaster = 0;
}

void incrementBeatCounters(boolean triplet, Bar &obj) {
  beatMaster++;
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
    if (swingOn) readSwing(0);
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

    tripletMS = tempoMS * 4 / 3;
    tripletMetro.interval(tripletMS);
    tempoBPM = 60000 / (tempoMS * 4);   /// ms to BPM conversion, the 4 is there because every DB "beat" is actually an eight note
    updateLCD(false, tempoBPM, false); // display new tempo on lcd, update only the BPM
  }
}

//---------------------- Digital -------------------//
void checkDigitalIO(Bar & obj) {
  checkHighPriorityButtons(obj);
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
