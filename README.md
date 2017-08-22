# About

Simple tool for arduino for controlling the ADF4350 using a serial command line interface.

Commands:

    freq FREQUENCY

Where FREQUENCY is the frequency in Hz as a float number, e.g. 200e6 for 200Mhz.

# Wiring

SPI SCK -> CLK
SPI MOSI -> DATA
10 -> LE
3.3V -> CE

# Copyright

Includes code for frequency settings from https://github.com/mathiashelsen/ADF4350_RPi

MIT License

Copyright (c) 2018 Innovationgarage AS
Copyright (c) 2017 Mathias Helsen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
