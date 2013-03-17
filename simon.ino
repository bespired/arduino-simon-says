#include "./Pitches.h"

// notes in the melody: ( length, tone, led )
int melody[] = { 4, NOTE_C4,1,  8, NOTE_G3,2,  8, NOTE_G3,2,  4, NOTE_A3,3,  4, NOTE_G3,4,  4, 0,0,  4, NOTE_B3,5,  4, NOTE_C4,6  };
int leds[]   = { 0, 12, 11, 10, 9, 8, 13 }; // pin config output LEDS ( Green/Red on 8, 13 );
int buts[]   = { 0, 6, 5, 4, 3 };           // pin config input buttons.
int notes[]  = { 0, NOTE_E2, NOTE_C3, NOTE_E3, NOTE_A3 };

int speakerPin = 7;
int pressed = 0;

int  modus       = 0;

char seq[512];
char cur_length  = 0;
char play_pos    = 0;
char check_pos   = 0;
char next_led    = 0;

void setup() {
  // assign pins
  pinMode( speakerPin , OUTPUT);     

  for( int p=1; p < 8; p++ ){ 
    pinMode( leds[p]  , OUTPUT);
    digitalWrite(leds[p], LOW);       
  }   

  for( int p=1; p < 5; p++ ){ 
    pinMode( buts[p]  , INPUT);
  }
}



void loop() {

   int pressed = 0;
   next_led = random( 1, 5 );             // get a random every cycle, speed of user picks other sequence.
       
   switch ( modus ) {
    
     case 0:  // Intro 
       music( melody , sizeof(melody) );
       cur_length = 0;
       seq[ cur_length ] = next_led;
       modus = 1;
       delay( 300 );
     break;
     
     case 1:  // Display sequence
       cur_length++;
       displaySequence( cur_length );
       check_pos = 0;
       modus = 2;
     break;
     
     case 2:  // Check sequence
       pressed  = getPressed(); // non-blocking, but handling led and tone
       if ( pressed > 0 ){
         if ( pressed == seq[ check_pos ] ){  // all okay... next step...
           check_pos++;
         }else{  // oops... toBad...
           playToBad();
           delay( 500 );
           modus = 3;
         }
       }
       if ( check_pos >= cur_length ){
         // seq okay... play hooray, and add one pos to sequence
         seq[ cur_length ] = next_led; // the random from above... 
         playHooray();
         delay( 500 );
         modus = 1;
       }
      
     break;
     
     case 3: // Wait for new game
       // maybe display score... ( how? )
     
       pressed = getPressed();
       if ( pressed > 0 ){
         modus = 0;
         delay( 500 );
       }
     break;
   }

}

//*** ***//

int getPressed( void ){
    int pressed = 0;
    for( int p=1; p< 5; p++ ){
       if(  digitalRead( buts[p] ) == HIGH ){
         pressed = p;
       }  
    }
    if ( pressed > 0 ){ // if a button is pressed handle the light and tone.
      digitalWrite( leds[pressed], HIGH );
      tone( speakerPin, notes[pressed] );

      while ( digitalRead( buts[ pressed ] ) == HIGH ){
        delay( 100 );
      }
      digitalWrite( leds[pressed], LOW );
      noTone( speakerPin );
    }
  return pressed;
}

void playHooray(){
  delay ( 250 );
  digitalWrite( leds[6], HIGH ); // Green LED
  tone( speakerPin, NOTE_C4 );  
  delay ( 400 );
  digitalWrite( leds[6], LOW );
  noTone( speakerPin );
  delay ( 100 );
}

void playToBad(){
  delay ( 300 );
  digitalWrite( leds[5], HIGH ); // Red LED
  tone( speakerPin, NOTE_C2 );
  delay ( 500 );
  noTone( speakerPin );
  displaySequence( cur_length );
  delay ( 100 );
  digitalWrite( leds[5], LOW );
  delay ( 100 );
}

void displaySequence( int len ){
  
  for ( int s=0; s < len; s++ ){
    int dur  = 500 - len * 5;
    if ( dur < 200 ) dur = 200;
    
    int item = seq[ s ];
    digitalWrite( leds[ item ], HIGH );
    tone( speakerPin, notes[ item ] );
    delay( dur );
    digitalWrite( leds[ item ], LOW );
    noTone( speakerPin );
    delay( dur * 0.4 );
  }
  
}

void music( int seq[] , int len ){

   int music_len = len / sizeof(int) / 3;
   for ( int thisNote = 0; thisNote < music_len; thisNote++ ) {
     int noteDuration = 700 / seq[ thisNote * 3 ];
     int ledpoort = seq[thisNote * 3 + 2];
     if ( ledpoort > 0 ) digitalWrite( leds[ledpoort], HIGH );
     tone( speakerPin, seq[thisNote * 3 + 1], noteDuration );
     delay( noteDuration * 1.30 );
     noTone( speakerPin );
     if ( ledpoort > 0 ) digitalWrite( leds[ledpoort], LOW );
  }
  
}

