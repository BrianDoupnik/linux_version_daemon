/*
 * Filename     hw5.h
 * Date         4/19/2020
 * Author       Brian Doupnik
 * Email        bcd170030
 * Course       CS 3377.502 Spring 2020
 * Version      1.0
 * Copyright    2020, All Rights Reserved
 *
 * Description
 *      Standard header file defines everything
 *      needed for the rest of the program
 */

#ifndef HW5_H
#define HW5_H

#include <map>
#include <string>

#define BUFFER_SIZE 1024

enum keys {DAEMON, CONFIGFILE, VERBOSE, LOGFILE, PASSWORD, NUMVERSIONS, WATCHDIR};
extern int theSig;

void parseCmdToMap (int, char**, std::map<int, std::string>&);

const char* parseConfigToMap (std::string, std::map<int, std::string>&, bool);

void dirMon (std::map<int, std::string>&);

const char * sys (const char*);

#endif
