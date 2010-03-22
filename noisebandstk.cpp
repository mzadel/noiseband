
//
// noisebandstk.cpp
//

// /Users/mark/mcgill/research/thesis/researchfiles/phasetwo/garygranular/stk-4.3.0/src/SineWave.cpp
// /Users/mark/mcgill/research/thesis/researchfiles/phasetwo/garygranular/stk-4.3.0/

#include "SineWave.h"

//using namespace stk; // needed for newer stk

int main( void ) {

    StkFloat lengthseconds = 2;
    unsigned long numsamples = static_cast<unsigned long>(lengthseconds * Stk::sampleRate());
    unsigned long numpartials = 100; // FIXME partials per octave
    StkFloat lofreq = 100;
    StkFloat hifreq = 200;

    // final output
    StkFrames output(0.0, numsamples, 1);
    StkFrames iterationoutput(numsamples, 1);

    for ( unsigned long i = 0; i < numpartials; i++ ) {

        std::cout << "adding a sine" << std::endl;

        SineWave thissine;
        StkFrames thisoutput(0.0, numsamples, 1);

        thissine.setFrequency( 100 );
        thissine.tick( thisoutput );
        //output += thisoutput; // possible in newer stk

        // older stk needs to do this loop manually:
        for ( unsigned long j = 0; j < numsamples; j++ ) {
            output[j] += iterationoutput[i];
        }

    }

    std::cout << "done" << std::endl;

};


// vim:sw=4:ts=4:et:ai:ic:
