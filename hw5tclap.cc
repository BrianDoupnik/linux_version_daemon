/*
 * Filename     hw5tclap.cc
 * Date         4/19/2020
 * Author       Brian Doupnik
 * Email        bcd170030
 * Course       CS 3377.502 Spring 2020
 * Version      1.0
 * Copyright    2020, All Rights Reserved
 *
 * Description
 *      Uses tclap to parse the command line arguments
 */

#include "hw5.h"

#include <tclap/CmdLine.h>
#include <tclap/SwitchArg.h>
#include <tclap/UnlabeledValueArg.h>

void parseCmdToMap(int argc, char** argv, std::map<int, std::string> &argmap)
{
  TCLAP::CmdLine cmd("Program 5", ' ', "1.0");
  
  TCLAP::SwitchArg daemonSwitch ("d", "daemon", "Run in daemon mode (forks to run as daemon).", cmd, false);
  TCLAP::UnlabeledValueArg<std::string> configArg ("configFile", "The name of the config file to use", false, "cs3377dirmond.conf", "config file name", false);
  cmd.add( &configArg );
  
  cmd.parse(argc, argv);
  
  if(daemonSwitch.getValue())
    argmap[DAEMON] = "true";
  else
    argmap[DAEMON] = "false";

  argmap[CONFIGFILE] = configArg.getValue();
  return;
}
