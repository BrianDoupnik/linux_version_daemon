/*
 * Filename     hw5main.cc
 * Date         4/19/2020
 * Author       Brian Doupnik
 * Email        bcd170030
 * Course       CS 3377.502 Spring 2020
 * Version      1.0
 * Copyright    2020, All Rights Reserved
 *
 * Description
 *      Main method that handles reading in
 *      information and forks if need be
 */

#include <iostream> //needed for printing
#include <string.h> //needed for strcmp
#include <unistd.h> //needed for fork

#include "hw5.h"

using namespace std;

void daemonMode (map<int, string> &);

int theSig;

int main (int argc, char ** argv)
{
  theSig = 0;
  map<int, string> argMap;
  parseCmdToMap ( argc, argv, argMap );
  
  const char* parseResult = parseConfigToMap (argMap[CONFIGFILE], argMap, false);

  if(!(parseResult == NULL))
  {
    cerr << parseResult << endl;
    return 1;
  }
  
  if(strcmp(argMap[VERBOSE].c_str(), "true") == 0)
  {
    cout << "Daemon: " << argMap[DAEMON] << endl;
    cout << "Config File: " << argMap[CONFIGFILE] << endl;
    cout << "Verbose: " << argMap[VERBOSE] << endl;
    cout << "Log file: " << argMap[LOGFILE] << endl;
    cout << "Password: " << argMap[PASSWORD] << endl;
    cout << "Number of Versions: " << argMap[NUMVERSIONS] << endl;
    cout << "Directory to Watch: " << argMap[WATCHDIR] << endl;
  }
  else
  {
    cout << "Directory to Watch: " << argMap[WATCHDIR] << endl;
  }

  
  if(strcmp(argMap[DAEMON].c_str(), "true") == 0)
  {
    pid_t forkValue;
    forkValue = fork();
    
    if(forkValue == -1)
    {
      cerr << "Fork failed." << endl;
      return 1;
    }
    else if(forkValue == 0)
    {
      dirMon(argMap);
      return 0;
    }
    else
    {
      return 0;
    }
    
    cerr << "Not caught by loop." << endl;
    return 1;
  }
  else
  {
    dirMon(argMap);
  }
  
  return 0;
}
