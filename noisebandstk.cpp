
//
// noisebandstk.cpp
//

// time to compute broadband noise:
//  StkFloat lofreq = 10;
//  StkFloat hifreq = 22000;
//  StkFloat partialsperoctave = 1500.0;
//  computing 16654 partials
//  real    14m46.404s

// FIXME / TODO
// take args on the command line
// doesn't sound white; maybe you need to correct for power as well?  lower
//   freqs will have more power; maybe have to divide it out
//   http://classes.yale.edu/Fractals/CA/OneOverF/PowerSpectrum/PowerSpectrum.html

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

StkFloat getrandfreq( StkFloat lofreq, StkFloat hifreq ) {

    StkFloat randval = rand() / (RAND_MAX + 1.0);

    return pow(hifreq/lofreq, randval) * lofreq;

}

StkFloat numoctaves( StkFloat lofreq, StkFloat hifreq ) {
    // return the number of octaves spanned by (lofreq,hifreq)
    return log2( hifreq / lofreq );
}

int main( void ) {

    StkFloat lengthseconds = 2;
    unsigned long numsamples = static_cast<unsigned long>(lengthseconds * Stk::sampleRate());
    StkFloat lofreq = 10;
    StkFloat hifreq = 22000;
    StkFloat partialsperoctave = 1500.0;
    unsigned long numpartials = static_cast<unsigned long>(partialsperoctave * numoctaves( lofreq, hifreq ));

    // final output
    StkFrames output(0.0, numsamples, 1);

    // sine ugen
    SineWave thissine;

    std::cout << "computing " << numpartials << " partials" << std::endl;

    for ( unsigned long i = 0; i < numpartials; i++ ) {

        //std::cout << "adding a sine" << std::endl;
        std::cout << "." << std::flush;

        if ( i % 20 == 0 )
            std::cout << " " << i << " " << std::flush;

        thissine.setFrequency( getrandfreq( lofreq, hifreq ) );

        // older stk needs to do this loop manually:
        // newer stk you can just tick on an intermediate buffer and += it into
        // the main output
        for ( unsigned long j = 0; j < numsamples; j++ ) {
            // NB we're assuming here that we're not going to overflow the
            // doubles
            output[j] += thissine.tick();
        }

    }

    std::cout << "done computing" << std::endl;

    normalize( output );

    std::cout << "done normalising" << std::endl;

    std::string outfilename( "outfile.wav" );

    // write to file
    FileWrite outfile( outfilename, 1, FileWrite::FILE_WAV, Stk::STK_FLOAT64 );
    outfile.write( output );
    outfile.close();

    std::cout << "wrote to " << outfilename << std::endl;

#if 0
    // just print out the values
    for ( unsigned long j = 0; j < numsamples; j++ ) {
        std::cout << j << " " << output[j] << std::endl;
    }
#endif

    std::cout << "all done" << std::endl;

}


// vim:sw=4:ts=4:et:ai:ic:
