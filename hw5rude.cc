/*
 * Filename     hw5rude.cc
 * Date         4/19/2020
 * Author       Brian Doupnik
 * Email        bcd170030
 * Course       CS 3377.502 Spring 2020
 * Version      1.0
 * Copyright    2020, All Rights Reserved
 *
 * Description
 *      Uses RudeConfig to parse the config file
 */

#include "hw5.h"

#include <rude/config.h>
#include <string.h>

const char* parseConfigToMap (std::string configName, std::map<int, std::string> &configMap, bool calledAtRuntime)
{
  rude::Config config;

  config.load(configName.c_str());

  const char* errorMsg = config.getError();
  if(strcmp(errorMsg, "") != 0)
    return errorMsg;

  if(!config.setSection("Parameters", false))
    return "No [Parameters] section.";

  if(config.exists("Verbose"))
    configMap[VERBOSE] = config.getStringValue("Verbose");
  else
    return "Verbose does not exist.";

  if(config.exists("LogFile"))
    configMap[LOGFILE] = config.getStringValue("LogFile");
  else
    return "LogFile does not exist.";


  if(config.exists("Password"))
  {
    if(calledAtRuntime == false)
      configMap[PASSWORD] = config.getStringValue("Password");
    else if(configMap[PASSWORD] != config.getStringValue("Password"))
      return "Tried to change password at runtime.";
  }
  else 
    return "Password does not exist.";

  if(config.exists("NumVersions"))
    configMap[NUMVERSIONS] = config.getStringValue("NumVersions");
  else
    return "Versions does not exist.";

  if(config.exists("WatchDir"))
  {

    if(calledAtRuntime == false)
      configMap[WATCHDIR] = config.getStringValue("WatchDir");
    else if(configMap[WATCHDIR] != config.getStringValue("WatchDir"))
      return "Tried to change watch directory at runtime.";
  }
  else
    return "WatchDir does not exist.";

  return NULL;
}
