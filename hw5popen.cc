/*
 * Filename     hw5popen.cc
 * Date         4/19/2020
 * Author       Brian Doupnik
 * Email        bcd170030
 * Course       CS 3377.502 Spring 2020
 * Version      1.0
 * Copyright    2020, All Rights Reserved
 *
 * Description
 *      Handles shell input and output
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "hw5.h"

using namespace std;

const char* sys (const char* command)
{
  FILE *output_from_command;
  char tmpbuffer[BUFFER_SIZE];
  char *line_p;

  output_from_command = popen(command, "r");
  if(!output_from_command)
    return NULL;

  line_p = fgets(tmpbuffer, BUFFER_SIZE, output_from_command);
  pclose(output_from_command);

  return line_p;
}
