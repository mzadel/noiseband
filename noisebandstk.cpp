
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
//   yes -- if you compare the spectrum to pink noise, it's the same; it's a
//   linear plot wrt log frequency
//   http://classes.yale.edu/Fractals/CA/OneOverF/PowerSpectrum/PowerSpectrum.html
//   http://en.wikipedia.org/wiki/Colors_of_noise#Power-law_noise
//   http://en.wikipedia.org/wiki/Power_spectral_density
// try making rand freq linear to see if you get white noise

#include "SineWave.h"
#include "FileWrite.h"

#include  <getopt.h>

//using namespace stk; // needed for newer stk

enum { WHITENOISE = 0, PINKNOISE };

static struct option long_options[] = {
    {"length",      required_argument, 0, 'l'},
    {"startfreq",   required_argument, 0, 's'},
    {"endfreq",     required_argument, 0, 'e'},
    {"ppo",         required_argument, 0, 'p'}, // partials per octave
    {"numpartials", required_argument, 0, 'n'}, // total number of partials
    {"white",       no_argument,       0, 'w'},
    {"pink",        no_argument,       0, 'k'},
    {"outfile",     required_argument, 0, 'o'},
    {0, 0, 0, 0}
};

void normalize( StkFrames& buffer )
{

    // this code is from FileWvIn class

    size_t i;
    StkFloat max = 0.0;

    for ( i=0; i<buffer.size(); i++ ) {
        if ( fabs( buffer[i] ) > max )
            max = (StkFloat) fabs((double) buffer[i]);
    }

    std::cout << "normalize(): max amplitude found is " << max << std::endl;

    if (max > 0.0) {
        max = 1.0 / max;
        for ( i=0; i<buffer.size(); i++ )
            buffer[i] *= max;
    }

}

StkFloat getlograndfreq( StkFloat lofreq, StkFloat hifreq ) {

    StkFloat randval = rand() / (RAND_MAX + 1.0);

    return pow(hifreq/lofreq, randval) * lofreq;

}

StkFloat getlinrandfreq( StkFloat lofreq, StkFloat hifreq ) {

    StkFloat randval = rand() / (RAND_MAX + 1.0);

    return randval * (hifreq-lofreq) + lofreq;

}

StkFloat numoctaves( StkFloat lofreq, StkFloat hifreq ) {
    // return the number of octaves spanned by (lofreq,hifreq)
    return log2( hifreq / lofreq );
}

int main( int argc, char **argv ) {

    StkFloat lengthseconds = 2;
    StkFloat lofreq = 10;
    StkFloat hifreq = 22000;
    StkFloat partialsperoctave = 1500.0;
    unsigned long numpartials = static_cast<unsigned long>(partialsperoctave * numoctaves( lofreq, hifreq ));
    unsigned int noisetype = WHITENOISE;
    std::string outfilename( "outfile.wav" );

    // -(option parsing)--------------------------------------------------------

    int optionfound = -1;
    while( 1 ) {

        int option_index = 0; // getopt_long stores the option index here.

        optionfound = getopt_long (argc, argv, "l:s:e:p:n:wko:", long_options, &option_index);

        // end of options
        if (optionfound == -1)
            break;

        switch (optionfound) {

            case 'l':
                lengthseconds = atof( optarg );
                break;

            case 's':
                lofreq = atof( optarg );
                break;

            case 'e':
                hifreq = atof( optarg );
                break;

            case 'p':
                numpartials = static_cast<unsigned long>(atof(optarg) * numoctaves( lofreq, hifreq ));
                // FIXME assumes the lo/hi freqs have been specified; fix that by putting this at the end somehow
                break;

            case 'n':
                numpartials = atol(optarg);
                break;

            case 'w':
                noisetype = WHITENOISE;
                break;

            case 'k':
                noisetype = PINKNOISE;
                break;

            case 'o':
                outfilename = optarg;
                break;

            default:
                abort();

        }

    };

    unsigned long numsamples = static_cast<unsigned long>(lengthseconds * Stk::sampleRate());

    std::cout << "lengthseconds " << lengthseconds << std::endl;
    std::cout << "numsamples " << numsamples  << std::endl;
    std::cout << "lofreq " << lofreq  << std::endl;
    std::cout << "hifreq " << hifreq  << std::endl;
    std::cout << "partialsperoctave " << partialsperoctave  << std::endl;
    std::cout << "numpartials " << numpartials << std::endl;
    if ( noisetype == WHITENOISE )
        std::cout << "noisetype " << "WHITENOISE" << std::endl;
    else if  ( noisetype == PINKNOISE )
        std::cout << "noisetype " << "PINKNOISE" << std::endl;
    std::cout << "outfilename " << outfilename << std::endl;




    // -(main routine)----------------------------------------------------------
#if 0
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

        if ( noisetype == WHITENOISE ) {
            thissine.setFrequency( getlinrandfreq( lofreq, hifreq ) );
        } else { // PINKNOISE
            thissine.setFrequency( getlograndfreq( lofreq, hifreq ) );
        }

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
#endif
}


// vim:sw=4:ts=4:et:ai:ic:
