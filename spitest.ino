#include <SPI.h>


#define COMMAND_BUFFER_LENGTH        100                        //length of serial buffer for incoming commands
char   CommandLine[COMMAND_BUFFER_LENGTH + 1];                 //Read commands into this buffer from Serial.  +1 in length for a termination char

bool getCommandLineFromSerialPort(char * commandLine);

void setup() {
  Serial.begin(57600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  begin();
  setFrequency(151.0e6);
  setFrequency(151.336e6);
}

void loop() {
  char *cmd;
  bool received = getCommandLineFromSerialPort(CommandLine);
  if (received) {
    cmd = strtok(CommandLine, ", \n");
    if (!strcmp(cmd, "freq")) {
      setFrequency(readFloat());
    } else {
      serprintf("Unknown command: %s\n", cmd);
    }
  }
}

