#include <Metro.h>
#include <wavTrigger.h>
#include <AltSoftSerial.h>
#include <avr/sleep.h>

Metro seqMetro(300);        // Sequencer state machine interval timer
Metro readAnalogInputs(100);      // Read potentiometer values
Metro readDigitalInputs(100);     // Read switch values

//WAV Trigger will only play WAV files formatted as 16-bit, stereo, 44.1kHz, and there can be no meta-data (non-audio data) in the file before the audio data.
//Make sure the file name starts with a 3 digit number.
wavTrigger wTrig;             // Our WAV Trigger object

//----------------------------------------------------------------------------------PATTERN CREATION-----------------------------------------------------------------------------//
// Use the congas patterns for toms too, which you can switch out with the kit selection
const word hatclosed_p1 PROGMEM  = 0b1010100010101000;
const word hatopen_p1 PROGMEM    = 0b0000001000000010;
const word rim_p1 PROGMEM        = 0b0000100000001000;
const word shaker_p1 PROGMEM     = 0b0000000000000000;
const word cymbal_p1 PROGMEM     = 0b0000000000000000;
const word kick_p1 PROGMEM       = 0b1000000100000000;
const word snare_p1 PROGMEM      = 0b0000000000000000;
const word clap_p1 PROGMEM       = 0b0000000000000000;
const word cowbell_p1 PROGMEM    = 0b1001001000000000;
const word claves_p1 PROGMEM     = 0b0000000000000000;
const word congahigh_p1 PROGMEM  = 0b0000000000000100;
const word congamid_p1 PROGMEM   = 0b0000000000000010;
const word congalow_p1 PROGMEM   = 0b0000000000000001;
const word pattern1[] PROGMEM = {hatclosed_p1, hatopen_p1, rim_p1, shaker_p1, cymbal_p1, kick_p1, snare_p1, clap_p1, cowbell_p1, claves_p1, congahigh_p1, congamid_p1, congalow_p1};

const word hatclosed_p1B PROGMEM = 0b1010100010101000;
const word hatopen_p1B PROGMEM   = 0b0000001000000010;
const word rim_p1B PROGMEM       = 0b0000100000001000;
const word shaker_p1B PROGMEM    = 0b0000000000000000;
const word cymbal_p1B PROGMEM    = 0b0000000000000000;
const word kick_p1B PROGMEM      = 0b1000000100000000;
const word snare_p1B PROGMEM     = 0b0000000000000000;
const word clap_p1B PROGMEM      = 0b0000000000000000;
const word cowbell_p1B PROGMEM   = 0b1001001000000000;
const word claves_p1B PROGMEM    = 0b0000000000000000;
const word congahigh_p1B PROGMEM = 0b0000000000000100;
const word congamid_p1B PROGMEM  = 0b0000000000000010;
const word congalow_p1B PROGMEM  = 0b0000000000000001;
const word pattern1B[] PROGMEM = {hatclosed_p1B, hatopen_p1B, rim_p1B, shaker_p1B, cymbal_p1B, kick_p1B, snare_p1B, clap_p1B, cowbell_p1B, claves_p1B, congahigh_p1B, congamid_p1B, congalow_p1B};

const word hatclosed_p2 PROGMEM  = 0b0000000000000000;
const word hatopen_p2 PROGMEM    = 0b0010000000100000;
const word rim_p2 PROGMEM        = 0b0000000000000000;
const word shaker_p2 PROGMEM     = 0b1111111111111111;
const word cymbal_p2 PROGMEM     = 0b0000000000000000;
const word kick_p2 PROGMEM       = 0b1000000110010001;
const word snare_p2 PROGMEM      = 0b0000100000001000;
const word clap_p2 PROGMEM       = 0b0000000000000000;
const word cowbell_p2 PROGMEM    = 0b0000000000000000;
const word claves_p2 PROGMEM     = 0b0000000000000000;
const word congahigh_p2 PROGMEM  = 0b0000000000000000;
const word congamid_p2 PROGMEM   = 0b0000000000000000;
const word congalow_p2 PROGMEM   = 0b0000000000000000;
const word pattern2[] PROGMEM = {hatclosed_p2, hatopen_p2, rim_p2, shaker_p2, cymbal_p2, kick_p2, snare_p2, clap_p2, cowbell_p2, claves_p2, congahigh_p2, congamid_p2, congalow_p2};

const word hatclosed_p2B PROGMEM = 0b0000000000000000;
const word hatopen_p2B PROGMEM   = 0b0010000000100000;
const word rim_p2B PROGMEM       = 0b0000000000000000;
const word shaker_p2B PROGMEM    = 0b1111111111111111;
const word cymbal_p2B PROGMEM    = 0b0000000000000000;
const word kick_p2B PROGMEM      = 0b1000000110010001;
const word snare_p2B PROGMEM     = 0b0000100000001000;
const word clap_p2B PROGMEM      = 0b0000000000000000;
const word cowbell_p2B PROGMEM   = 0b0000000000000000;
const word claves_p2B PROGMEM    = 0b0000000000000000;
const word congahigh_p2B PROGMEM = 0b0000000000000000;
const word congamid_p2B PROGMEM  = 0b0000000000000000;
const word congalow_p2B PROGMEM  = 0b0000000000000000;
const word pattern2B[] PROGMEM = {hatclosed_p2B, hatopen_p2B, rim_p2B, shaker_p2B, cymbal_p2B, kick_p2B, snare_p2B, clap_p2B, cowbell_p2B, claves_p2B, congahigh_p2B, congamid_p2B, congalow_p2B};

const word hatclosed_p3 PROGMEM  = 0b0000000000000000;
const word hatopen_p3 PROGMEM    = 0b0000000000000000;
const word rim_p3 PROGMEM        = 0b0000000000000000;
const word shaker_p3 PROGMEM     = 0b0000000000000000;
const word cymbal_p3 PROGMEM     = 0b0000000000000000;
const word kick_p3 PROGMEM       = 0b0000000000000000;
const word snare_p3 PROGMEM      = 0b0000000000000000;
const word clap_p3 PROGMEM       = 0b0000000000000000;
const word cowbell_p3 PROGMEM    = 0b0000000000000000;
const word claves_p3 PROGMEM     = 0b1001001000101000;
const word congahigh_p3 PROGMEM  = 0b0000100100000000;
const word congamid_p3 PROGMEM   = 0b1000000000010000;
const word congalow_p3 PROGMEM   = 0b0000000000000010;
const word pattern3[] PROGMEM = {hatclosed_p3, hatopen_p3, rim_p3, shaker_p3, cymbal_p3, kick_p3, snare_p3, clap_p3, cowbell_p3, claves_p3, congahigh_p3, congamid_p3, congalow_p3};

const word hatclosed_p3B PROGMEM = 0b0000000000000000;
const word hatopen_p3B PROGMEM   = 0b0000000000000000;
const word rim_p3B PROGMEM       = 0b0000000000000000;
const word shaker_p3B PROGMEM    = 0b0000000000000000;
const word cymbal_p3B PROGMEM    = 0b0000000000000000;
const word kick_p3B PROGMEM      = 0b0000000000000000;
const word snare_p3B PROGMEM     = 0b0000000000000000;
const word clap_p3B PROGMEM      = 0b0000000000000000;
const word cowbell_p3B PROGMEM   = 0b0000000000000000;
const word claves_p3B PROGMEM    = 0b1001001000101000;
const word congahigh_p3B PROGMEM = 0b0000100100000000;
const word congamid_p3B PROGMEM  = 0b1000000000010000;
const word congalow_p3B PROGMEM  = 0b0000000000000010;
const word pattern3B[] PROGMEM = {hatclosed_p3B, hatopen_p3B, rim_p3B, shaker_p3B, cymbal_p3B, kick_p3B, snare_p3B, clap_p3B, cowbell_p3B, claves_p3B, congahigh_p3B, congamid_p3B, congalow_p3B};

const word hatclosed_p4 PROGMEM  = 0b0000000000000000;
const word hatopen_p4 PROGMEM    = 0b0000000000000000;
const word rim_p4 PROGMEM        = 0b0000000000000000;
const word shaker_p4 PROGMEM     = 0b1011111110111111;
const word cymbal_p4 PROGMEM     = 0b0000000000000000;
const word kick_p4 PROGMEM       = 0b1001001000000000;
const word snare_p4 PROGMEM      = 0b0000000000000000;
const word clap_p4 PROGMEM       = 0b0000000000001000;
const word cowbell_p4 PROGMEM    = 0b0000000000000000;
const word claves_p4 PROGMEM     = 0b1101101101101100;
const word congahigh_p4 PROGMEM  = 0b0000000000000000;
const word congamid_p4 PROGMEM   = 0b0000000000000000;
const word congalow_p4 PROGMEM   = 0b0000000000000000;
const word pattern4[] PROGMEM = {hatclosed_p4, hatopen_p4, rim_p4, shaker_p4, cymbal_p4, kick_p4, snare_p4, clap_p4, cowbell_p4, claves_p4, congahigh_p4, congamid_p4, congalow_p4};

const word hatclosed_p4B PROGMEM = 0b0000000000000000;
const word hatopen_p4B PROGMEM   = 0b0000000000000000;
const word rim_p4B PROGMEM       = 0b0000000000000000;
const word shaker_p4B PROGMEM    = 0b1011111110111111;
const word cymbal_p4B PROGMEM    = 0b0000000000000000;
const word kick_p4B PROGMEM      = 0b1001001000000000;
const word snare_p4B PROGMEM     = 0b0000000000000000;
const word clap_p4B PROGMEM      = 0b0000000000001000;
const word cowbell_p4B PROGMEM   = 0b0000000000000000;
const word claves_p4B PROGMEM    = 0b1101101101101100;
const word congahigh_p4B PROGMEM = 0b0000000000000000;
const word congamid_p4B PROGMEM  = 0b0000000000000000;
const word congalow_p4B PROGMEM  = 0b0000000000000000;
const word pattern4B[] PROGMEM = {hatclosed_p4B, hatopen_p4B, rim_p4B, shaker_p4B, cymbal_p4B, kick_p4B, snare_p4B, clap_p4B, cowbell_p4B, claves_p4B, congahigh_p4B, congamid_p4B, congalow_p4B};

const word hatclosed_p5 PROGMEM  = 0b1111111111111111;
const word hatopen_p5 PROGMEM    = 0b0000000000000000;
const word rim_p5 PROGMEM        = 0b0000000000000000;
const word shaker_p5 PROGMEM     = 0b0000000000000000;
const word cymbal_p5 PROGMEM     = 0b0000000000000000;
const word kick_p5 PROGMEM       = 0b1000100010001000;
const word snare_p5 PROGMEM      = 0b0000000000000000;
const word clap_p5 PROGMEM       = 0b0000100000001000;
const word cowbell_p5 PROGMEM    = 0b0000000000000000;
const word claves_p5 PROGMEM     = 0b0000000000000011;
const word congahigh_p5 PROGMEM  = 0b0000000000000000;
const word congamid_p5 PROGMEM   = 0b0000000000000000;
const word congalow_p5 PROGMEM   = 0b0000000000000000;
const word pattern5[] PROGMEM = {hatclosed_p5, hatopen_p5, rim_p5, shaker_p5, cymbal_p5, kick_p5, snare_p5, clap_p5, cowbell_p5, claves_p5, congahigh_p5, congamid_p5, congalow_p5};

const word hatclosed_p5B PROGMEM = 0b1111111111111111;
const word hatopen_p5B PROGMEM   = 0b0000000000000000;
const word rim_p5B PROGMEM       = 0b0000000000000000;
const word shaker_p5B PROGMEM    = 0b0000000000000000;
const word cymbal_p5B PROGMEM    = 0b0000000000000000;
const word kick_p5B PROGMEM      = 0b1000100010001000;
const word snare_p5B PROGMEM     = 0b0000000000000000;
const word clap_p5B PROGMEM      = 0b0000100000001000;
const word cowbell_p5B PROGMEM   = 0b0000000000000000;
const word claves_p5B PROGMEM    = 0b0000000000000011;
const word congahigh_p5B PROGMEM = 0b0000000000000000;
const word congamid_p5B PROGMEM  = 0b0000000000000000;
const word congalow_p5B PROGMEM  = 0b0000000000000000;
const word pattern5B[] PROGMEM = {hatclosed_p5B, hatopen_p5B, rim_p5B, shaker_p5B, cymbal_p5B, kick_p5B, snare_p5B, clap_p5B, cowbell_p5B, claves_p5B, congahigh_p5B, congamid_p5B, congalow_p5B};

const word hatclosed_p6 PROGMEM  = 0b0010001100100111;
const word hatopen_p6 PROGMEM    = 0b0000000000000000;
const word rim_p6 PROGMEM        = 0b0000000000000000;
const word shaker_p6 PROGMEM     = 0b0000000000000000;
const word cymbal_p6 PROGMEM     = 0b0000000000000000;
const word kick_p6 PROGMEM       = 0b1000100010001000;
const word snare_p6 PROGMEM      = 0b0000000000000000;
const word clap_p6 PROGMEM       = 0b0000010000000100;
const word cowbell_p6 PROGMEM    = 0b0000000000000000;
const word claves_p6 PROGMEM     = 0b0000000000000000;
const word congahigh_p6 PROGMEM  = 0b1000000000000000;
const word congamid_p6 PROGMEM   = 0b0001001000000000;
const word congalow_p6 PROGMEM   = 0b0000000001000000;
const word pattern6[] PROGMEM = {hatclosed_p6, hatopen_p6, rim_p6, shaker_p6, cymbal_p6, kick_p6, snare_p6, clap_p6, cowbell_p6, claves_p6, congahigh_p6, congamid_p6, congalow_p6};

const word hatclosed_p6B PROGMEM = 0b0010001100100111;
const word hatopen_p6B PROGMEM   = 0b0000000000000000;
const word rim_p6B PROGMEM       = 0b0000000000000000;
const word shaker_p6B PROGMEM    = 0b0000000000000000;
const word cymbal_p6B PROGMEM    = 0b0000000000000000;
const word kick_p6B PROGMEM      = 0b1000100010001000;
const word snare_p6B PROGMEM     = 0b0000000000000000;
const word clap_p6B PROGMEM      = 0b0000010000000100;
const word cowbell_p6B PROGMEM   = 0b0000000000000000;
const word claves_p6B PROGMEM    = 0b0000000000000000;
const word congahigh_p6B PROGMEM = 0b1000000000000000;
const word congamid_p6B PROGMEM  = 0b0001001000000000;
const word congalow_p6B PROGMEM  = 0b0000000001000000;
const word pattern6B[] PROGMEM = {hatclosed_p6B, hatopen_p6B, rim_p6B, shaker_p6B, cymbal_p6B, kick_p6B, snare_p6B, clap_p6B, cowbell_p6B, claves_p6B, congahigh_p6B, congamid_p6B, congalow_p6B};

const word hatclosed_p7 PROGMEM  = 0b0000000000000000;
const word hatopen_p7 PROGMEM    = 0b0000000000000000;
const word rim_p7 PROGMEM        = 0b0000000000000000;
const word shaker_p7 PROGMEM     = 0b1111111111111111;
const word cymbal_p7 PROGMEM     = 0b0000000000000000;
const word kick_p7 PROGMEM       = 0b1000010010000100;
const word snare_p7 PROGMEM      = 0b0000000000000000;
const word clap_p7 PROGMEM       = 0b0000100000010000;
const word cowbell_p7 PROGMEM    = 0b0000000000000000;
const word claves_p7 PROGMEM     = 0b1100110011001100;
const word congahigh_p7 PROGMEM  = 0b0000000000000000;
const word congamid_p7 PROGMEM   = 0b0000000000000000;
const word congalow_p7 PROGMEM   = 0b0000000000000000;
const word pattern7[] PROGMEM = {hatclosed_p7, hatopen_p7, rim_p7, shaker_p7, cymbal_p7, kick_p7, snare_p7, clap_p7, cowbell_p7, claves_p7, congahigh_p7, congamid_p7, congalow_p7};

const word hatclosed_p7B PROGMEM = 0b0000000000000000;
const word hatopen_p7B PROGMEM   = 0b0000000000000000;
const word rim_p7B PROGMEM       = 0b0000000000000000;
const word shaker_p7B PROGMEM    = 0b1111111111111111;
const word cymbal_p7B PROGMEM    = 0b0000000000000000;
const word kick_p7B PROGMEM      = 0b1000010010000100;
const word snare_p7B PROGMEM     = 0b0000000000000000;
const word clap_p7B PROGMEM      = 0b0000100000010000;
const word cowbell_p7B PROGMEM   = 0b0000000000000000;
const word claves_p7B PROGMEM    = 0b1100110011001100;
const word congahigh_p7B PROGMEM = 0b0000000000000000;
const word congamid_p7B PROGMEM  = 0b0000000000000000;
const word congalow_p7B PROGMEM  = 0b0000000000000000;
const word pattern7B[] PROGMEM = {hatclosed_p7B, hatopen_p7B, rim_p7B, shaker_p7B, cymbal_p7B, kick_p7B, snare_p7B, clap_p7B, cowbell_p7B, claves_p7B, congahigh_p7B, congamid_p7B, congalow_p7B};

const word hatclosed_p8 PROGMEM  = 0b1100110011001100;
const word hatopen_p8 PROGMEM    = 0b0010001000100010;
const word rim_p8 PROGMEM        = 0b0000000000000000;
const word shaker_p8 PROGMEM     = 0b0000000000000000;
const word cymbal_p8 PROGMEM     = 0b0000000000000000;
const word kick_p8 PROGMEM       = 0b1010100010001000;
const word snare_p8 PROGMEM      = 0b0000100000001000;
const word clap_p8 PROGMEM       = 0b0000000000000000;
const word cowbell_p8 PROGMEM    = 0b0000000000000000;
const word claves_p8 PROGMEM     = 0b0000000000000000;
const word congahigh_p8 PROGMEM  = 0b0000000000000000;
const word congamid_p8 PROGMEM   = 0b0000000000000000;
const word congalow_p8 PROGMEM   = 0b0000000000000000;
const word pattern8[] PROGMEM = {hatclosed_p8, hatopen_p8, rim_p8, shaker_p8, cymbal_p8, kick_p8, snare_p8, clap_p8, cowbell_p8, claves_p8, congahigh_p8, congamid_p8, congalow_p8};

const word hatclosed_p8B PROGMEM = 0b1100110011001100;
const word hatopen_p8B PROGMEM   = 0b0010001000100010;
const word rim_p8B PROGMEM       = 0b0000000000000000;
const word shaker_p8B PROGMEM    = 0b0000000000000000;
const word cymbal_p8B PROGMEM    = 0b0000000000000000;
const word kick_p8B PROGMEM      = 0b1010100010001000;
const word snare_p8B PROGMEM     = 0b0000100000001000;
const word clap_p8B PROGMEM      = 0b0000000000000000;
const word cowbell_p8B PROGMEM   = 0b0000000000000000;
const word claves_p8B PROGMEM    = 0b0000000000000000;
const word congahigh_p8B PROGMEM = 0b0000000000000000;
const word congamid_p8B PROGMEM  = 0b0000000000000000;
const word congalow_p8B PROGMEM  = 0b0000000000000000;
const word pattern8B[] PROGMEM = {hatclosed_p8B, hatopen_p8B, rim_p8B, shaker_p8B, cymbal_p8B, kick_p8B, snare_p8B, clap_p8B, cowbell_p8B, claves_p8B, congahigh_p8B, congamid_p8B, congalow_p8B};

const word hatclosed_p9 PROGMEM  = 0b0000000000000000;
const word hatopen_p9 PROGMEM    = 0b1000100010001000;
const word rim_p9 PROGMEM        = 0b0000000000000000;
const word shaker_p9 PROGMEM     = 0b0111011101110111;
const word cymbal_p9 PROGMEM     = 0b0000000000000000;
const word kick_p9 PROGMEM       = 0b0000000000000000;
const word snare_p9 PROGMEM      = 0b0000000000000000;
const word clap_p9 PROGMEM       = 0b0000000000000000;
const word cowbell_p9 PROGMEM    = 0b1010110101011010;
const word claves_p9 PROGMEM     = 0b0000000000000000;
const word congahigh_p9 PROGMEM  = 0b0000000000000000;
const word congamid_p9 PROGMEM   = 0b0000000000000000;
const word congalow_p9 PROGMEM   = 0b0000000000000000;
const word pattern9[] PROGMEM = {hatclosed_p9, hatopen_p9, rim_p9, shaker_p9, cymbal_p9, kick_p9, snare_p9, clap_p9, cowbell_p9, claves_p9, congahigh_p9, congamid_p9, congalow_p9};

const word hatclosed_p9B PROGMEM = 0b0000000000000000;
const word hatopen_p9B PROGMEM   = 0b1000100010001000;
const word rim_p9B PROGMEM       = 0b0000000000000000;
const word shaker_p9B PROGMEM    = 0b0111011101110111;
const word cymbal_p9B PROGMEM    = 0b0000000000000000;
const word kick_p9B PROGMEM      = 0b0000000000000000;
const word snare_p9B PROGMEM     = 0b0000000000000000;
const word clap_p9B PROGMEM      = 0b0000000000000000;
const word cowbell_p9B PROGMEM   = 0b1010110101011010;
const word claves_p9B PROGMEM    = 0b0000000000000000;
const word congahigh_p9B PROGMEM = 0b0000000000000000;
const word congamid_p9B PROGMEM  = 0b0000000000000000;
const word congalow_p9B PROGMEM  = 0b0000000000000000;
const word pattern9B[] PROGMEM = {hatclosed_p9B, hatopen_p9B, rim_p9B, shaker_p9B, cymbal_p9B, kick_p9B, snare_p9B, clap_p9B, cowbell_p9B, claves_p9B, congahigh_p9B, congamid_p9B, congalow_p9B};

const word hatclosed_p10 PROGMEM = 0b1101110111011101;
const word hatopen_p10 PROGMEM   = 0b0000000000000000;
const word rim_p10 PROGMEM       = 0b0000000000000000;
const word shaker_p10 PROGMEM    = 0b0000000000000000;
const word cymbal_p10 PROGMEM    = 0b0000000000000000;
const word kick_p10 PROGMEM      = 0b1010001010100100;
const word snare_p10 PROGMEM     = 0b0000100101001001;
const word clap_p10 PROGMEM      = 0b0000000000000000;
const word cowbell_p10 PROGMEM   = 0b0000000000000000;
const word claves_p10 PROGMEM    = 0b0000000000000000;
const word congahigh_p10 PROGMEM = 0b0000000000000000;
const word congamid_p10 PROGMEM  = 0b0000000000000000;
const word congalow_p10 PROGMEM  = 0b0000000000000000;
const word pattern10[] PROGMEM = {hatclosed_p10, hatopen_p10, rim_p10, shaker_p10, cymbal_p10, kick_p10, snare_p10, clap_p10, cowbell_p10, claves_p10, congahigh_p10, congamid_p10, congalow_p10};

const word hatclosed_p10B PROGMEM = 0b1101110111011101;
const word hatopen_p10B PROGMEM  = 0b0000000000000000;
const word rim_p10B PROGMEM      = 0b0000000000000000;
const word shaker_p10B PROGMEM   = 0b0000000011111111;
const word cymbal_p10B PROGMEM   = 0b0000000000000000;
const word kick_p10B PROGMEM     = 0b1010001010100100;
const word snare_p10B PROGMEM    = 0b0000100101001001;
const word clap_p10B PROGMEM     = 0b0000000000000000;
const word cowbell_p10B PROGMEM  = 0b0000000000000000;
const word claves_p10B PROGMEM   = 0b0000000000000000;
const word congahigh_p10B PROGMEM = 0b0000000000000000;
const word congamid_p10B PROGMEM = 0b0000000000000000;
const word congalow_p10B PROGMEM = 0b0000000000000000;
const word pattern10B[] PROGMEM = {hatclosed_p10B, hatopen_p10B, rim_p10B, shaker_p10B, cymbal_p10B, kick_p10B, snare_p10B, clap_p10B, cowbell_p10B, claves_p10B, congahigh_p10B, congamid_p10B, congalow_p10B};

const word hatclosed_p11 PROGMEM = 0b0000000000000000;
const word hatopen_p11 PROGMEM   = 0b1011101110111010;
const word rim_p11 PROGMEM       = 0b0000000000000000;
const word shaker_p11 PROGMEM    = 0b0000000000000000;
const word cymbal_p11 PROGMEM    = 0b0000000000000000;
const word kick_p11 PROGMEM      = 0b0000000000000000;
const word snare_p11 PROGMEM     = 0b0000000000000000;
const word clap_p11 PROGMEM      = 0b0000000000000000;
const word cowbell_p11 PROGMEM   = 0b0000000000000000;
const word claves_p11 PROGMEM    = 0b0000000000000000;
const word congahigh_p11 PROGMEM = 0b0000000000000000;
const word congamid_p11 PROGMEM  = 0b0000000000000000;
const word congalow_p11 PROGMEM  = 0b0000000000000000;
const word pattern11[] PROGMEM = {hatclosed_p11, hatopen_p11, rim_p11, shaker_p11, cymbal_p11, kick_p11, snare_p11, clap_p11, cowbell_p11, claves_p11, congahigh_p11, congamid_p11, congalow_p11};

const word hatclosed_p11B PROGMEM = 0b0000000000000000;
const word hatopen_p11B PROGMEM  = 0b1011101110111010;
const word rim_p11B PROGMEM      = 0b0000000000000000;
const word shaker_p11B PROGMEM   = 0b0000000000000000;
const word cymbal_p11B PROGMEM   = 0b0000000000000000;
const word kick_p11B PROGMEM     = 0b0000000000000000;
const word snare_p11B PROGMEM    = 0b0000000000000000;
const word clap_p11B PROGMEM     = 0b0000000000100010;
const word cowbell_p11B PROGMEM  = 0b0000000000000000;
const word claves_p11B PROGMEM   = 0b0000000000000000;
const word congahigh_p11B PROGMEM = 0b0000000000000000;
const word congamid_p11B PROGMEM = 0b0000000000000000;
const word congalow_p11B PROGMEM = 0b0000000000000000;
const word pattern11B[] PROGMEM = {hatclosed_p11B, hatopen_p11B, rim_p11B, shaker_p11B, cymbal_p11B, kick_p11B, snare_p11B, clap_p11B, cowbell_p11B, claves_p11B, congahigh_p11B, congamid_p11B, congalow_p11B};

const word hatclosed_p12 PROGMEM = 0b1010101010101010;
const word hatopen_p12 PROGMEM   = 0b0000000000000000;
const word rim_p12 PROGMEM       = 0b0000000000000000;
const word shaker_p12 PROGMEM    = 0b0000000000000000;
const word cymbal_p12 PROGMEM    = 0b1000100010001000;
const word kick_p12 PROGMEM      = 0b0000000000000000;
const word snare_p12 PROGMEM     = 0b0001000000001000;
const word clap_p12 PROGMEM      = 0b0000000000000000;
const word cowbell_p12 PROGMEM   = 0b0000000000000000;
const word claves_p12 PROGMEM    = 0b0000000000000000;
const word congahigh_p12 PROGMEM = 0b0000000000000000;
const word congamid_p12 PROGMEM  = 0b0000000000000000;
const word congalow_p12 PROGMEM  = 0b0000000000000000;
const word pattern12[] PROGMEM = {hatclosed_p12, hatopen_p12, rim_p12, shaker_p12, cymbal_p12, kick_p12, snare_p12, clap_p12, cowbell_p12, claves_p12, congahigh_p12, congamid_p12, congalow_p12};

const word hatclosed_p12B PROGMEM = 0b1010101010101010;
const word hatopen_p12B PROGMEM  = 0b0000000000000000;
const word rim_p12B PROGMEM      = 0b0101000001010100;
const word shaker_p12B PROGMEM   = 0b0000000000000000;
const word cymbal_p12B PROGMEM   = 0b1000100010001000;
const word kick_p12B PROGMEM     = 0b0000000000000000;
const word snare_p12B PROGMEM    = 0b0001000000001000;
const word clap_p12B PROGMEM     = 0b0000000000000000;
const word cowbell_p12B PROGMEM  = 0b0000000000000000;
const word claves_p12B PROGMEM   = 0b0000000000000000;
const word congahigh_p12B PROGMEM = 0b0000000000000000;
const word congamid_p12B PROGMEM = 0b0000000000000000;
const word congalow_p12B PROGMEM = 0b0000000000000000;
const word pattern12B[] PROGMEM = {hatclosed_p12B, hatopen_p12B, rim_p12B, shaker_p12B, cymbal_p12B, kick_p12B, snare_p12B, clap_p12B, cowbell_p12B, claves_p12B, congahigh_p12B, congamid_p12B, congalow_p12B};

const word hatclosed_p13 PROGMEM = 0b0000000000000000;
const word hatopen_p13 PROGMEM   = 0b0000000000000000;
const word rim_p13 PROGMEM       = 0b0000000000000000;
const word shaker_p13 PROGMEM    = 0b0000000000000000;
const word cymbal_p13 PROGMEM    = 0b0000000000000000;
const word kick_p13 PROGMEM      = 0b1001001001001000;
const word snare_p13 PROGMEM     = 0b0000000000000000;
const word clap_p13 PROGMEM      = 0b0000100000001011;
const word cowbell_p13 PROGMEM   = 0b0000000000000000;
const word claves_p13 PROGMEM    = 0b0000000000000000;
const word congahigh_p13 PROGMEM = 0b0000000000000000;
const word congamid_p13 PROGMEM  = 0b0000000000000000;
const word congalow_p13 PROGMEM  = 0b0000000000000000;
const word pattern13[] PROGMEM = {hatclosed_p13, hatopen_p13, rim_p13, shaker_p13, cymbal_p13, kick_p13, snare_p13, clap_p13, cowbell_p13, claves_p13, congahigh_p13, congamid_p13, congalow_p13};

const word hatclosed_p13B PROGMEM = 0b0000000000000000;
const word hatopen_p13B PROGMEM  = 0b0000000000000000;
const word rim_p13B PROGMEM      = 0b0000000000000000;
const word shaker_p13B PROGMEM   = 0b1000000000000000;
const word cymbal_p13B PROGMEM   = 0b0000000000000000;
const word kick_p13B PROGMEM     = 0b1001001001001000;
const word snare_p13B PROGMEM    = 0b0000000000000000;
const word clap_p13B PROGMEM     = 0b0000100000001011;
const word cowbell_p13B PROGMEM  = 0b0000000000000000;
const word claves_p13B PROGMEM   = 0b0000000000000000;
const word congahigh_p13B PROGMEM = 0b0000000000000000;
const word congamid_p13B PROGMEM = 0b0000000000000000;
const word congalow_p13B PROGMEM = 0b0000000000000000;
const word pattern13B[] PROGMEM = {hatclosed_p13B, hatopen_p13B, rim_p13B, shaker_p13B, cymbal_p13B, kick_p13B, snare_p13B, clap_p13B, cowbell_p13B, claves_p13B, congahigh_p13B, congamid_p13B, congalow_p13B};

const word hatclosed_p14 PROGMEM = 0b1000100010001000;
const word hatopen_p14 PROGMEM   = 0b0000000000000000;
const word rim_p14 PROGMEM       = 0b0000000010100100;
const word shaker_p14 PROGMEM    = 0b0000000000000000;
const word cymbal_p14 PROGMEM    = 0b0000000000000000;
const word kick_p14 PROGMEM      = 0b1000000000000000;
const word snare_p14 PROGMEM     = 0b0000100101001001;
const word clap_p14 PROGMEM      = 0b0000000000000000;
const word cowbell_p14 PROGMEM   = 0b0000000000000000;
const word claves_p14 PROGMEM    = 0b0000000000000000;
const word congahigh_p14 PROGMEM = 0b0000000000000000;
const word congamid_p14 PROGMEM  = 0b0000000000000000;
const word congalow_p14 PROGMEM  = 0b0000000000000000;
const word pattern14[] PROGMEM = {hatclosed_p14, hatopen_p14, rim_p14, shaker_p14, cymbal_p14, kick_p14, snare_p14, clap_p14, cowbell_p14, claves_p14, congahigh_p14, congamid_p14, congalow_p14};

const word hatclosed_p14B PROGMEM = 0b1000100010001000;
const word hatopen_p14B PROGMEM  = 0b0000000000000000;
const word rim_p14B PROGMEM      = 0b0000000010100100;
const word shaker_p14B PROGMEM   = 0b0000000000000000;
const word cymbal_p14B PROGMEM   = 0b0000000000000000;
const word kick_p14B PROGMEM     = 0b1000000000000000;
const word snare_p14B PROGMEM    = 0b0000100101001001;
const word clap_p14B PROGMEM     = 0b0000000000000000;
const word cowbell_p14B PROGMEM  = 0b0010000101000000;
const word claves_p14B PROGMEM   = 0b0000000000000000;
const word congahigh_p14B PROGMEM = 0b0000000000000000;
const word congamid_p14B PROGMEM = 0b0000000000000000;
const word congalow_p14B PROGMEM = 0b0000000000000000;
const word pattern14B[] PROGMEM = {hatclosed_p14B, hatopen_p14B, rim_p14B, shaker_p14B, cymbal_p14B, kick_p14B, snare_p14B, clap_p14B, cowbell_p14B, claves_p14B, congahigh_p14B, congamid_p14B, congalow_p14B};

const word hatclosed_p15 PROGMEM = 0b0000000000000000;
const word hatopen_p15 PROGMEM   = 0b0000000000000000;
const word rim_p15 PROGMEM       = 0b0000100000001000;
const word shaker_p15 PROGMEM    = 0b1111111111111111;
const word cymbal_p15 PROGMEM    = 0b0000000000000000;
const word kick_p15 PROGMEM      = 0b1000000000000000;
const word snare_p15 PROGMEM     = 0b0000000000000000;
const word clap_p15 PROGMEM      = 0b0000000000000000;
const word cowbell_p15 PROGMEM   = 0b0000000000000000;
const word claves_p15 PROGMEM    = 0b0000000000000000;
const word congahigh_p15 PROGMEM = 0b0010100000110000;
const word congamid_p15 PROGMEM  = 0b0100000001000000;
const word congalow_p15 PROGMEM  = 0b1000010110001000;
const word pattern15[] PROGMEM = {hatclosed_p15, hatopen_p15, rim_p15, shaker_p15, cymbal_p15, kick_p15, snare_p15, clap_p15, cowbell_p15, claves_p15, congahigh_p15, congamid_p15, congalow_p15};

const word hatclosed_p15B PROGMEM = 0b0000000000000000;
const word hatopen_p15B PROGMEM  = 0b0000000000000000;
const word rim_p15B PROGMEM      = 0b0000100000001000;
const word shaker_p15B PROGMEM   = 0b1111111111111111;
const word cymbal_p15B PROGMEM   = 0b0000000000000000;
const word kick_p15B PROGMEM     = 0b1000000000000000;
const word snare_p15B PROGMEM    = 0b0000000000000000;
const word clap_p15B PROGMEM     = 0b0000000000000000;
const word cowbell_p15B PROGMEM  = 0b1010000100010000;
const word claves_p15B PROGMEM   = 0b0000000000000000;
const word congahigh_p15B PROGMEM = 0b0010100000110000;
const word congamid_p15B PROGMEM = 0b0100000001000000;
const word congalow_p15B PROGMEM = 0b1000010110001000;
const word pattern15B[] PROGMEM = {hatclosed_p15B, hatopen_p15B, rim_p15B, shaker_p15B, cymbal_p15B, kick_p15B, snare_p15B, clap_p15B, cowbell_p15B, claves_p15B, congahigh_p15B, congamid_p15B, congalow_p15B};

const word hatclosed_p16 PROGMEM = 0b0000000000000000;
const word hatopen_p16 PROGMEM   = 0b0000000000000000;
const word rim_p16 PROGMEM       = 0b0000000000000000;
const word shaker_p16 PROGMEM    = 0b0000000000000000;
const word cymbal_p16 PROGMEM    = 0b0000000000000000;
const word kick_p16 PROGMEM      = 0b0000000000000000;
const word snare_p16 PROGMEM     = 0b0000000000000000;
const word clap_p16 PROGMEM      = 0b0000000000000000;
const word cowbell_p16 PROGMEM   = 0b0000000000000000;
const word claves_p16 PROGMEM    = 0b0000000000000000;
const word congahigh_p16 PROGMEM = 0b1010000101010000;
const word congamid_p16 PROGMEM  = 0b0000110000001010;
const word congalow_p16 PROGMEM  = 0b0000000000000000;
const word pattern16[] PROGMEM = {hatclosed_p16, hatopen_p16, rim_p16, shaker_p16, cymbal_p16, kick_p16, snare_p16, clap_p16, cowbell_p16, claves_p16, congahigh_p16, congamid_p16, congalow_p16};

const word hatclosed_p16B PROGMEM = 0b0000000000000000;
const word hatopen_p16B PROGMEM  = 0b0000000000000000;
const word rim_p16B PROGMEM      = 0b0000000000000000;
const word shaker_p16B PROGMEM   = 0b0000000000000000;
const word cymbal_p16B PROGMEM   = 0b0000000000000000;
const word kick_p16B PROGMEM     = 0b0000000000000000;
const word snare_p16B PROGMEM    = 0b0000100000001000;
const word clap_p16B PROGMEM     = 0b0000000000000000;
const word cowbell_p16B PROGMEM  = 0b0000000000000000;
const word claves_p16B PROGMEM   = 0b0000000000000000;
const word congahigh_p16B PROGMEM = 0b1010000101010000;
const word congamid_p16B PROGMEM = 0b0000110000001010;
const word congalow_p16B PROGMEM = 0b0000000000000000;
const word pattern16B[] PROGMEM = {hatclosed_p16B, hatopen_p16B, rim_p16B, shaker_p16B, cymbal_p16B, kick_p16B, snare_p16B, clap_p16B, cowbell_p16B, claves_p16B, congahigh_p16B, congamid_p16B, congalow_p16B};

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
Samples samples_3;

// ------------------------------------------------------------------    DECLARE PINS AND CONSTANTS ------------------------------------------------------------//
const int VOLUME_PIN = A0;
const int TEMPO_PIN = A1;

const int POWER_PIN = 2;
const int PAUSE_SWITCH_PIN = 1;
const int AB_PIN = 0;
const int FILL_BUTTON_PIN = 3;
const int SWING_SWITCH_PIN = 13;

const int SIXTEEN_SWITCH_PIN_A = 6;
const int SIXTEEN_SWITCH_PIN_B = 7;
const int SIXTEEN_SWITCH_PIN_C = 5;
const int SIXTEEN_SWITCH_PIN_D = 4;

const int THREE_SWITCH_PIN_A = 10;
const int THREE_SWITCH_PIN_B = 11;
const int THREE_SWITCH_PIN_C = 12;

#define NUM_TRACKS 44
char trackVolumes[NUM_TRACKS] = {0}; // char data type range is -128 to 127 which is perfect for volume range of -70 to 10

//--------------------------------------------------------------VARIABLE DECLARATION & INITIALIZATION------------------------------------------------------//
boolean swingOn;
boolean stutterOn = false;
boolean pauseOn;
boolean pattern_mod;

int tempoMS;
int volume;
int swingMS;
int tempoBPM;

word beatMaster = 0;
word beat = 0;
int currentPattern;
int currentSampleSet;

boolean debug = false;

//------------------------------------------------------------------- SETUP --------------------------------------------------------------------------------------------------//
void setup() {
  // Serial monitor for debugging

  if (debug) Serial.begin(9600);

  pinMode(SIXTEEN_SWITCH_PIN_A, INPUT_PULLUP);
  pinMode(SIXTEEN_SWITCH_PIN_B, INPUT_PULLUP);
  pinMode(SIXTEEN_SWITCH_PIN_C, INPUT_PULLUP);
  pinMode(SIXTEEN_SWITCH_PIN_D, INPUT_PULLUP);

  pinMode(THREE_SWITCH_PIN_A, INPUT_PULLUP);
  pinMode(THREE_SWITCH_PIN_B, INPUT_PULLUP);
  pinMode(THREE_SWITCH_PIN_C, INPUT_PULLUP);

  pinMode(POWER_PIN, INPUT_PULLUP);
  pinMode(PAUSE_SWITCH_PIN, INPUT_PULLUP);
  pinMode(AB_PIN, INPUT_PULLUP);
  pinMode(FILL_BUTTON_PIN, INPUT_PULLUP);
  pinMode(SWING_SWITCH_PIN, INPUT_PULLUP);

  swingOn =  digitalRead(SWING_SWITCH_PIN) == HIGH ? true : false;
  pattern_mod =  digitalRead(AB_PIN) == HIGH ? true : false;
  
  if (debug){
    pauseOn =  false;
  }else{
    pauseOn =  digitalRead(PAUSE_SWITCH_PIN) == HIGH ? true : false;
  }

  delay(500); // wait for the WAV Trigger to finish reset before trying to send commands.
  wTrig.start(); // WAV Trigger startup at 57600
  delay(10);
  wTrig.stopAllTracks();// Send a stop-all command and reset the sample-rate offset, in case we have reset while the WAV Trigger was already playing.

  // enable track reporting (recieve data from the WAV Trigger)
  wTrig.setReporting(true);
  delay(100);
  // get version/track info from the wav trigger
  char gWTrigVersion[VERSION_STRING_LEN];    // WAV Trigger version string
  if (wTrig.getVersion(gWTrigVersion, VERSION_STRING_LEN)) {
    if (debug) Serial.print(gWTrigVersion);
    if (debug) Serial.print("\n");
    int gNumTracks = wTrig.getNumTracks();
    if (debug) Serial.print("Number of tracks = ");
    if (debug) Serial.println(gNumTracks);
  }
  else
    if (debug) Serial.print("WAV Trigger response not available");
  delay(100);

  setSamples();
  setTrackVolumes();

  readTempo(1); // read vol and tempo values
  readVolume(1);

  // play startup sound
  wTrig.trackPlayPoly(100);
  delay(2000);
}

//--------------------------SETUP METHODS--------------------------//

void setSamples() {
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

  // SAMPLES 3 TODO
  samples_3.hatclosed = 31;
  samples_3.hatopen = 32;
  samples_3.rim = 33;
  samples_3.shaker = 34;
  samples_3.cymbal = 35;

  samples_3.kick = 36;
  samples_3.snare = 37;
  samples_3.clap = 38;
  samples_3.cowbell = 39;

  samples_3.claves = 40;
  samples_3.congahigh = 41;
  samples_3.congamid = 42;
  samples_3.congalow = 43;
}

void setTrackVolumes() {
  // PRESET VOLUMES FOR EACH SAMPLE
  trackVolumes[1] = 0;
  trackVolumes[2] = 0;
  trackVolumes[3] = 0;
  trackVolumes[4] = 0;
  trackVolumes[5] = 0;
  trackVolumes[6] = 0;
  trackVolumes[7] = 0;
  trackVolumes[8] = -7;
  trackVolumes[9] = 0;
  trackVolumes[10] = 0;
  trackVolumes[11] = 0;
  trackVolumes[12] = 0;
  trackVolumes[13] = 0;
  trackVolumes[14] = 0;
  trackVolumes[15] = 0;
  trackVolumes[16] = 0;
  trackVolumes[17] = 0;
  trackVolumes[18] = 0;
  trackVolumes[19] = 0;
  trackVolumes[20] = 0;
  trackVolumes[21] = 0;
  trackVolumes[22] = 0;
  trackVolumes[23] = 0;
  trackVolumes[24] = 0;
  trackVolumes[25] = 0;
  trackVolumes[26] = 0;
  trackVolumes[27] = 0;
  trackVolumes[28] = 0;
  trackVolumes[29] = 0;
  trackVolumes[30] = 0;
  trackVolumes[31] = 0;
  trackVolumes[32] = 0;
  trackVolumes[33] = 0;
  trackVolumes[34] = 0;
  trackVolumes[35] = 0;
  trackVolumes[36] = 0;
  trackVolumes[37] = 0;
  trackVolumes[38] = 0;
  trackVolumes[39] = 0;
  trackVolumes[40] = 0;
  trackVolumes[41] = 0;
  trackVolumes[42] = 0;
  trackVolumes[43] = 0;

  for ( int i = 0 ; i < NUM_TRACKS ; i++)
    wTrig.trackGain(i, trackVolumes[i]);
}

//------------------------------------------------------------------------------------MAIN LOOP------------------------------------------------------------------------//

void loop() {

  //  activePattern
  //  activeSamples

  if (!pauseOn) {
    if (seqMetro.check() == 1) {
      setSwing();

      int inverseMaster = 15 - beat;

      boolean activeHatClosed = bitRead(activePattern.hatclosed, inverseMaster);
      boolean activeHatOpen = bitRead(activePattern.hatopen, inverseMaster);
      boolean activeRim = bitRead(activePattern.rim, inverseMaster);
      boolean activeShaker = bitRead(activePattern.shaker, inverseMaster);
      boolean activeCymbal = bitRead(activePattern.cymbal, inverseMaster);

      boolean activeKick = bitRead(activePattern.kick, inverseMaster);
      boolean activeCSnare = bitRead(activePattern.snare, inverseMaster);
      boolean activeClap = bitRead(activePattern.clap, inverseMaster);
      boolean activeCowbell = bitRead(activePattern.cowbell, inverseMaster);

      boolean activeClaves = bitRead(activePattern.claves, inverseMaster);
      boolean activeCongaHigh = bitRead(activePattern.congahigh, inverseMaster);
      boolean activeCongaMid = bitRead(activePattern.congamid, inverseMaster);
      boolean activeCongaLow = bitRead(activePattern.congalow, inverseMaster);

      if (activeHatClosed) wTrig.trackPlayPoly(activeSamples.hatclosed);
      if (activeHatOpen) wTrig.trackPlayPoly(activeSamples.hatopen);
      if (activeRim) wTrig.trackPlayPoly(activeSamples.rim);
      if (activeShaker) wTrig.trackPlayPoly(activeSamples.shaker);
      if (activeCymbal) wTrig.trackPlayPoly(activeSamples.cymbal);

      if (activeKick) wTrig.trackPlayPoly(activeSamples.kick);
      if (activeCSnare) wTrig.trackPlayPoly(activeSamples.snare);
      if (activeClap) wTrig.trackPlayPoly(activeSamples.clap);
      if (activeCowbell) wTrig.trackPlayPoly(activeSamples.cowbell);

      if (activeClaves) wTrig.trackPlayPoly(activeSamples.claves);
      if (activeCongaHigh) wTrig.trackPlayPoly(activeSamples.congahigh);
      if (activeCongaMid) wTrig.trackPlayPoly(activeSamples.congamid);
      if (activeCongaLow) wTrig.trackPlayPoly(activeSamples.congalow);

      incrementBeatCounters();
    }
    if (stutterOn) fill();
  }

  checkDigitalIO();
  checkAnalogIO();
}

//------------------------------------------------------------------------------------LOOP METHODS------------------------------------------------------------------------//

void switchSamples(int s) {
  if (s == 1) transferSamples(samples_808);
  if (s == 2) transferSamples(samples_mixed);
  if (s == 3) transferSamples(samples_3);
}

void transferSamples(Samples &passedSamples) {
  activeSamples = passedSamples;
}

void changePattern(int p) {
  if (!pattern_mod) {
    if (p == 1) transferPatternMem(pattern1);
    if (p == 2) transferPatternMem(pattern2);
    if (p == 3) transferPatternMem(pattern3);
    if (p == 4) transferPatternMem(pattern4);

    if (p == 5) transferPatternMem(pattern5);
    if (p == 6) transferPatternMem(pattern6);
    if (p == 7) transferPatternMem(pattern7);
    if (p == 8) transferPatternMem(pattern8);

    if (p == 9) transferPatternMem(pattern9);
    if (p == 10) transferPatternMem(pattern10);
    if (p == 11) transferPatternMem(pattern11);
    if (p == 12) transferPatternMem(pattern12);

    if (p == 13) transferPatternMem(pattern13);
    if (p == 14) transferPatternMem(pattern14);
    if (p == 15) transferPatternMem(pattern15);
    if (p == 16) transferPatternMem(pattern16);
  } else {
    if (p == 1) transferPatternMem(pattern1B);
    if (p == 2) transferPatternMem(pattern2B);
    if (p == 3) transferPatternMem(pattern3B);
    if (p == 4) transferPatternMem(pattern4B);

    if (p == 5) transferPatternMem(pattern5B);
    if (p == 6) transferPatternMem(pattern6B);
    if (p == 7) transferPatternMem(pattern7B);
    if (p == 8) transferPatternMem(pattern8B);

    if (p == 9) transferPatternMem(pattern9B);
    if (p == 10) transferPatternMem(pattern10B);
    if (p == 11) transferPatternMem(pattern11B);
    if (p == 12) transferPatternMem(pattern12B);

    if (p == 13) transferPatternMem(pattern13B);
    if (p == 14) transferPatternMem(pattern14B);
    if (p == 15) transferPatternMem(pattern15B);
    if (p == 16) transferPatternMem(pattern16B);
  }
}

void transferPatternMem(const word memPattern[]) {
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
  beat = 14;
}

void startAtBeat0() {
  beatMaster = 0;
  beat = 0;
}

void incrementBeatCounters() {
  beatMaster = (beatMaster == 15) ? 0 : beatMaster + 1;
  beat = (beat == 15) ? 0 : beat + 1;
}

void setSwing() {
  if (swingOn) {
    if (beatMaster % 2 == 1)seqMetro.interval(tempoMS - swingMS);
    else seqMetro.interval(tempoMS + swingMS);
  }
}


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
  volumeIntermediate = analogRead(VOLUME_PIN);
  if ( abs( volumeLastIntermediate - volumeIntermediate) > 10 || firstCall) {
    volumeLastIntermediate = volumeIntermediate;
    volume = map(volumeIntermediate, 0, 1023, 4, -70);  // -70 to 4 DB is the full range of the wav trigger
    wTrig.masterGain(volume);
    if (debug) Serial.print("Volume: "); 
    if (debug) Serial.println(volume);
  }
}

void readTempo(boolean firstCall) {
  // read analog input for tempo - if significantly changed update the tempo
  static int tempoIntermediate;
  static int tempoLastIntermediate;
  tempoIntermediate = analogRead(TEMPO_PIN);
  if ( abs( tempoLastIntermediate - tempoIntermediate ) > 10 || firstCall) {
    tempoLastIntermediate = tempoIntermediate;
    tempoMS = map(tempoIntermediate, 1023, 0, 320, 80);
    seqMetro.interval(tempoMS);
    tempoBPM = 60000 / (tempoMS * 4);   /// ms to BPM conversion, the 4 is there because every DB "beat" is actually an eight note
    swingMS = tempoMS/3;
    if (debug) Serial.print("Tempo: "); Serial.println(tempoBPM);
  }
}

//---------------------- Digital -------------------//
void checkDigitalIO() {
  if (readDigitalInputs.check() == 1) {  // if minumum time has passed the inputs are read
    read16Switch();
    read3Switch();
    if (!debug)readPauseSwitch();
    readSwingSwitch();
    readFillButton();
    if (!debug)readABSwitch();
    readPowerSwitch();
  }
}

int read16Switch() {

  int pinA = 0;
  int pinB = 0;
  int pinC = 0;
  int pinD = 0;

  if ( digitalRead(SIXTEEN_SWITCH_PIN_A) == LOW ) pinA = 1;
  if ( digitalRead(SIXTEEN_SWITCH_PIN_B) == LOW ) pinB = 1;
  if ( digitalRead(SIXTEEN_SWITCH_PIN_C) == LOW ) pinC = 1;
  if ( digitalRead(SIXTEEN_SWITCH_PIN_D) == LOW ) pinD = 1;

  // debugging wiring
  //Serial.println(String(pinA) + " " + String(pinB) + " " + String(pinC) + " " +  String(pinD));

  int switch_pattern = 1 * pinA + 2 * pinB + 4 * pinC + 8 * pinD + 1;

  if (currentPattern != switch_pattern) {
    currentPattern = switch_pattern;
    changePattern(currentPattern);
    if (debug) Serial.print("changing pattern to ");
    if (debug) Serial.println(currentPattern);
  }
}

int read3Switch() {

  int switch_pattern;
  switch_pattern = digitalRead(THREE_SWITCH_PIN_A) == LOW ? 1 : switch_pattern;
  switch_pattern = digitalRead(THREE_SWITCH_PIN_B) == LOW ? 2 : switch_pattern;
  switch_pattern = digitalRead(THREE_SWITCH_PIN_C) == LOW ? 3 : switch_pattern;

  if (currentSampleSet != switch_pattern) {
    switchSamples(switch_pattern);
    currentSampleSet = switch_pattern;
    if (debug) Serial.print("changing sample set to ");
    if (debug) Serial.println(currentSampleSet);
  }
}

void readFillButton() {
  if ( digitalRead(FILL_BUTTON_PIN) == LOW && !stutterOn) {
    stutterOn = true;
    Serial.println("stutter ON");
  }
  if (digitalRead(FILL_BUTTON_PIN) == HIGH && stutterOn && beatMaster == 0) {
    stutterOn = false;
    beat = beatMaster;
    Serial.println("stutter OFF");
  }
}

void readSwingSwitch() {
  if ( digitalRead(SWING_SWITCH_PIN) == HIGH && !swingOn) {
    if (debug) Serial.println("Swing ON");//
    swingOn = true;
  }
  if (digitalRead(SWING_SWITCH_PIN) == LOW && swingOn) {
    if (debug) Serial.println("Swing OFF");
    seqMetro.interval(tempoMS);
    swingOn = false;
  }
}

void readABSwitch() {
  if ( digitalRead(AB_PIN) == HIGH && !pattern_mod) {
    if (debug) Serial.println("Pattern B");//
    pattern_mod = true;
    changePattern(currentPattern);
  }
  if (digitalRead(AB_PIN) == LOW && pattern_mod) {
    if (debug) Serial.println("Pattern A");
    pattern_mod = false;
    changePattern(currentPattern);
  }
}

void readPauseSwitch() {
  if ( digitalRead(PAUSE_SWITCH_PIN) == HIGH && !pauseOn) {
    pauseOn = true;
    seqMetro.reset();
    startAtBeat0();
  }
  if (digitalRead(PAUSE_SWITCH_PIN) == LOW && pauseOn) {
    pauseOn = false;
  }
}

void readPowerSwitch() {
  // got to sleep
  if ( digitalRead(POWER_PIN) == HIGH) {
    wTrig.trackPlayPoly(41);
    sleep_enable();
    attachInterrupt(digitalPinToInterrupt(POWER_PIN), wakeUp, LOW);
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    delay(1000);
    sleep_cpu();
  }
}

void wakeUp(){
  wTrig.trackPlayPoly(43);
  sleep_disable();
  detachInterrupt(digitalPinToInterrupt(POWER_PIN));
}
