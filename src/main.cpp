#include <iostream>
#include <ncurses.h>
// #include <thread>
// #include <unistd.h>
#// include <Morse.h>
#include <wiringPi.h>

using namespace std;


int main() {

	int wpm = 20;
	int initialwpm = 20;
	int change = 0;
//	int OldClkState = 1;
//	int NewClkState ;
//	int DataBitRead;

	const int SB0 = 1;
	const int SB1 = 2;
	
	wiringPiSetup();
	//void pullUpDnControl (int pin, int pud) ;

	pullUpDnControl(SB0, PUD_UP);
	pullUpDnControl(SB1, PUD_UP);
	digitalWrite(SB0, LOW);
	digitalWrite(SB1, LOW);
    pinMode(SB0, INPUT);
    pinMode(SB1, INPUT);

	int OldState = ((digitalRead(SB1) & 1 ) * 2 ) | (digitalRead(SB0) & 1 ) ;
	int NewState = OldState;

	cout << "Encoder Program" << endl; 

//
//  Try this:
//
//                           _______         _______       
//               Pin1 ______|       |_______|       |______ Pin1
// negative <---         _______         _______         __      --> positive
//               Pin2 __|       |_______|       |_______|   Pin2

		//	new	new	old	old
		//	pin2	pin1	pin2	pin1	Result
		//	----	----	----	----	------
		//	0	0	0	0	no movement
		//	0	0	0	1	+1
		//	0	0	1	0	-1
		//	0	0	1	1	+2  (assume pin1 edges only)
		//	0	1	0	0	-1
		//	0	1	0	1	no movement
		//	0	1	1	0	-2  (assume pin1 edges only)
		//	0	1	1	1	+1
		//	1	0	0	0	+1
		//	1	0	0	1	-2  (assume pin1 edges only)
		//	1	0	1	0	no movement
		//	1	0	1	1	-1
		//	1	1	0	0	+2  (assume pin1 edges only)
		//	1	1	0	1	-1
		//	1	1	1	0	+1
		//	1	1	1	1	no movement

//  Different state diagram, new state, old state
    //    new     new     old     old    
    //    pin2    pin1    pin2    pin1    Result
    //    ----    ----    ----    ----    ------
    //     0       0       0       0      no movement
    //     0       1       0       0      -1
    //     1       0       0       0      +1	
    //     1       1       0       0      +2 (assume pin1 edges only)	
                        
    //     0       1       0       1      no movement
    //     1       1       0       1      -1
    //     0       0       0       1      +1
	//     1       0       0       1      -2 (assume pin1 edges only)
                        
    //     1       1       1       1      no movement
    //     1       0       1       1      -1
    //     0       1       1       1      +1
    //     0       0       1       1      +2 (assume pin1 edges only)	
                        
    //     1       0       1       0      no movement
    //     0       0       1       0      -1
    //     1       1       1       0      +1
    //     0       1       1       0      -2 (assume pin1 edges only)	
/*
	// Simple, easy-to-read "documentation" version :-)
	//
	void update(void) {
		uint8_t s = state & 3;
		if (digitalRead(pin1)) s |= 4;
		if (digitalRead(pin2)) s |= 8;
		switch (s) {
			case 0: case 5: case 10: case 15:
				break;
			case 1: case 7: case 8: case 14:
				position++; break;
			case 2: case 4: case 11: case 13:
				position--; break;
			case 3: case 12:
				position += 2; break;
			default:
				position -= 2; break;
		}
		state = (s >> 2);
	}
*/
	while (1) {
		
		napms(5);
		if ( OldState != NewState ) {
			wpm = initialwpm + change/4 ;
			if ( wpm < 0 ) { 
				wpm = 0;
				change = -80;
			}
			if ( wpm > 40 ) {
				wpm = 40;
				change = 80;
			}
			std::cout << "WPM is now " << wpm << endl; 
			}
		OldState = NewState;
		NewState = ((digitalRead(SB1) & 1 ) * 2 ) | (digitalRead(SB0) & 1 ) ;
		
		switch (OldState) {
			case 0:
	//     0       0       0       0      no movement
	//     0       1       0       0      -1
	//     1       0       0       0      +1	
	//     1       1       0       0      +2

				if ( NewState == 1 ) change-- ;
				if ( NewState == 2 ) change++ ;
				//if ( NewState == 3 ) change += 2;
				break ;
			
			case 1:
	//     0       1       0       1      no movement
	//     1       1       0       1      -1
	//     0       0       0       1      +1
	//     1       0       0       1      -2 

				if ( NewState == 3 ) change-- ;
				if ( NewState == 0 ) change++ ;
				//if ( NewState == 2 ) change -= 2;
				break ;

			case 2:
	//     1       0       1       0      no movement
	//     0       0       1       0      -1
	//     1       1       1       0      +1
	//     0       1       1       0      -2 (assume pin1 edges only)	

				if ( NewState == 0 ) change-- ;
				if ( NewState == 3 ) change++ ;
				//if ( NewState == 1 ) change -= 2;
				break ;

			case 3:
	//     1       1       1       1      no movement
	//     1       0       1       1      -1
	//     0       1       1       1      +1
	//     0       0       1       1      +2 (assume pin1 edges only)	

				if ( NewState == 2 ) change-- ;
				if ( NewState == 1 ) change++ ;
				//if ( NewState == 0 ) wpm += 2;
				break ;



		}
	}
}
