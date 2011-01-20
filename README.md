
## Description

**noiseband** synthesizes audio noise using additive synthesis.  It can
generate a precise band of noise by adding together an arbitrary number of
partials.  This serves as a reasonable approximation of band-limited noise when
using a large number of partials.

It is implemented using classes from **STK** 4.3.0 by Perry Cook and Gary
Scavone (<https://ccrma.stanford.edu/software/stk/>).

This program does *not* work in real time.  On my machine, I can create two
seconds of noise spanning 10Hz to 22000Hz using 16654 partials in 1 minute, 36
seconds.

## Usage

    noiseband [OPTIONS]

Where the options are:

    -l <seconds>, --length <seconds>
        the output file length in seconds
        (default is 2.0 seconds)

    -s <freq>, --startfreq <freq>
        the starting (lowest) frequency of the noise band in Hertz
        (default is 10Hz)

    -e <freq>, --endfreq <freq>
        the ending (highest) frequency of the noise band in Hertz
        (default is 22000Hz)

    -p <number>, --ppo <number>
        the number of partials per octave.  this overrides option -n
        (default is 1500.0)

    -n <number>, --number <number>
        the total number of partials for the entire band.  this overrides option -p
        (default is 0)

    -w, --white
        use white noise
        (this is the default noise type)

    -k, --pink
        use pink noise

    -o <file.wav>, --outfile <file.wav>
        the output file name
        (default is outfile.wav)


## Remarks

The sampling rate is 44100 Hz, and the output wav files are mono, using 32-bit
floats.  Internal computations are done in terms of 64-bit floats (doubles).

## Compiling

    make

    For compiling on PowerPC, remove the __LITTLE_ENDIAN__ flag in the Makefile.

## Examples

FIXME


## License

noiseband.cpp is under the GPL, version 2
(<http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt>).

The remaining source files are part of STK, and are under their own license
(<https://ccrma.stanford.edu/software/stk/information.html>), copied here:

License

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

Any person wishing to distribute modifications to the Software is asked to send
the modifications to the original developer so that they can be incorporated
into the canonical version. This is, however, not a binding provision of this
license.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

## Author

noiseband.cpp was written by Mark Zadel.  The other source files were written
by their respective authors.

