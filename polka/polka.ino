// Play a file from the SD card in looping mode, from the SD card.
// Example program to demonstrate the use of the MIDFile library
//
// Hardware required:
//  SD card interface - change SD_SELECT for SPI comms

#include <SdFat.h>
#include <MD_MIDIFile.h>
#include <arduino-timer.h>

auto timer = timer_create_default(); // create a timer with default settings


#define DEBUGS(s)     Serial.print(s)
#define DEBUG(s, x)   { Serial.print(F(s)); Serial.print(x); }
#define DEBUGX(s, x)  { Serial.print(F(s)); Serial.print(x, HEX); }
#define SERIAL_RATE 57600

#define  SD_SELECT  4

//#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))

const char *loopfile = "polka_flute.mid";  // simple and short file
//blvrnew
//blvrnewloop
SdFat	SD;
MD_MIDIFile SMF;
//#define DUMP_DATA 1;


bool started = false;

//int G5[] = {2,3,4};
//int AH5[] = {2,4, 5};
//int D6[] = {2,3,4,5,6};
//int C6[] = {3};
//int F[] = {2,3,4,5};
//int A51[] = {2,3};
//int F5[] = {2,3,4,5};
//int AH6[] = {2,4,7};
//int A61[] = {2,3,7};




//int G5[] = {2,3,4};
//int AH5[] = {2,3};
//int D6[] = {2,3,4,5,6,7};
//int C6[] = {};
//int F[] = {2,3,4,5};
//int A51[] = {2,3};
//int F5[] = {2,3,4,5};
//int AH6[] = {2,3};
//int A61[] = {2,3};


void play(int [], bool, int);

void midiCallback(midi_event *pev)
{
  
int FH5[] = {2,3,4,5};
int CH5[] = {};
int GH5[] = {2,3,4};
int A51[] = {2,3};
int E5[] = {2,3,4,5,6};
int CH6[] = {};
int B51[] = {2};


  DEBUGX("\nM T", pev->track);
  DEBUG(":  Ch ", pev->channel);
  DEBUGX(" Data", sizeof(pev->data));
  DEBUGX(" Data0", pev->data[0]);
  DEBUGX(" Data1", pev->data[1]);
  DEBUGX(" Data2", pev->data[2]);
//
  int note = pev->data[1];
  int velocity = pev->data[2];
  int mDelay = pev->channel;
//
  DEBUG(" ", mDelay);
  DEBUG(" ", note);
  DEBUG(" ", velocity);
  Serial.println("--");

  switch(note) {
    case 66: // F#5
//    startVibe();
    play(FH5, velocity != 0, 4);
    break;
    case 61://C#5
    play(CH5,velocity != 0, 0);
    break;

    case 68://G#5
    DEBUG(" ON ", 74);
    play(GH5, velocity != 0,3);
    break;

    case 69: //A5
    play(A51, velocity != 0,2);
    break;


    case 64://E5
    play(E5, velocity != 0,5);
    break;

    case 73://C#6
    play(CH6, velocity != 0,0);
    break;

    case 71: // B5
    play(B51,velocity != 0,1);
    break;

    case 56: 
    
    
    break;
  
//    case 73: //A#6
//    play(AH6, velocity != 0,3);
//    break;
//
//    case 81://A6
//    play(A61, velocity != 0,3);
//    break;

    default:
      Serial.println("NOTHING");
    break;

  }
}


void play(int notes[], bool on, int arrsize) {

  if (on) {
    digitalWrite(8, LOW); //Air On
    for (int i = 0; i < arrsize;i++) {
      DEBUG(" ON", notes[i]);
      digitalWrite(notes[i], LOW);
    }
  } else {
    digitalWrite(8, HIGH); //Air Off
    for (int i = 0; i < arrsize;i++) {
      DEBUG(" OFF", notes[i]);
      digitalWrite(notes[i], HIGH);
    }
    delay(10);
  }
}

void sysexCallback(sysex_event *pev)
// Called by the MIDIFile library when a system Exclusive (sysex) file event needs 
// to be processed through the midi communications interface. Most sysex events cannot 
// really be processed, so we just ignore it here.
// This callback is set up in the setup() function.
{
  DEBUGS("\nS T");
  DEBUGS(pev->track);
  DEBUGS(": Data ");
  for (uint8_t i=0; i<pev->size; i++)
  {
    DEBUGS(pev->data[i]);
//    DEBUG(' ');
  }
}

void setup(void)
{
  
  int  err;
  Serial.begin(SERIAL_RATE);
  DEBUGS("\n[MidiFile Looper]");

  // Initialize SD
  if (!SD.begin(SD_SELECT, SPI_FULL_SPEED))
  {
    DEBUGS("\nSD init fail!");
    while (true) ;
  }

  // Initialize MIDIFile
  SMF.begin(&SD);
  SMF.setMidiHandler(midiCallback);
//  SMF.looping(true);
  SMF.setTempoAdjust(10);
  SMF.setSysexHandler(sysexCallback);

  // use the next file name and play it
  DEBUG("\nFile: ", loopfile);
  SMF.setFilename(loopfile);
  err = SMF.load(loopfile);
  if (err != 0)
  {
    DEBUG("\nSMF load Error ", err);
    while (true);
  }
//
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);


// call the toggle_led function every 500 millis (half second)
  

}

//void startVibe() 
//{
//
//  if (started == true) {
//    return;
//  }
//  started = true;
//  timer.every(500, vibe);
////  timer.every(580, vibeoff);
//  
//}

void vibeoff() {
  
}
void vibe() {
//  digitalWrite(9, LOW); //on
//  
//  delay(180);
//  digitalWrite(9, HIGH); //off
}

void loop(void)
{
  // play the file
  if (!SMF.isEOF())
  {
    SMF.getNextEvent();
  }
  timer.tick();
}
