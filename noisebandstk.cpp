
//
// noisebandstk.cpp
//

// /Users/mark/mcgill/research/thesis/researchfiles/phasetwo/garygranular/stk-4.3.0/src/SineWave.cpp
// /Users/mark/mcgill/research/thesis/researchfiles/phasetwo/garygranular/stk-4.3.0/

#include "SineWave.h"
#include "FileWrite.h"

//using namespace stk; // needed for newer stk

void normalize( StkFrames& buffer )
{

    // this code is from FileWvIn class

    size_t i;
    StkFloat max = 0.0;

    for ( i=0; i<buffer.size(); i++ ) {
        if ( fabs( buffer[i] ) > max )
            max = (StkFloat) fabs((double) buffer[i]);
    }

    if (max > 0.0) {
        max = 1.0 / max;
        for ( i=0; i<buffer.size(); i++ )
            buffer[i] *= max;
    }
}


int main( void ) {

    StkFloat lengthseconds = 2;
    unsigned long numsamples = static_cast<unsigned long>(lengthseconds * Stk::sampleRate());
    unsigned long numpartials = 100; // FIXME partials per octave
    StkFloat lofreq = 100;
    StkFloat hifreq = 200;

    // final output
    StkFrames output(0.0, numsamples, 1);

    for ( unsigned long i = 0; i < numpartials; i++ ) {

        //std::cout << "adding a sine" << std::endl;
        std::cout << "." << std::flush;

        // FIXME maybe factor this out since the starting phase wouldn't matter anyway
        SineWave thissine;
        thissine.setFrequency( 10 );

        // older stk needs to do this loop manually:
        // newer stk you can just tick on an intermediate buffer and += it into
        // the main output
        for ( unsigned long j = 0; j < numsamples; j++ ) {
            output[j] += thissine.tick();
        }

    }

    std::cout << "done computing" << std::endl;

#if 0
    std::string outfilename( "outfile.wav" );

    // write to file
    FileWrite outfile( outfilename, 1, FileWrite::FILE_WAV, Stk::STK_FLOAT32 );
    outfile.write( output );
    outfile.close();

    std::cout << "wrote to " << outfilename << std::endl;
#endif

    normalize( output );

    // just print out the values
    for ( unsigned long j = 0; j < numsamples; j++ ) {
        std::cout << j << " " << output[j] << std::endl;
    }

    std::cout << "all done" << std::endl;

};


// vim:sw=4:ts=4:et:ai:ic:
