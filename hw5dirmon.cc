/*
 * Filename     hw5dirmond.cc
 * Date         4/19/2020
 * Author       Brian Doupnik
 * Email        bcd170030
 * Course       CS 3377.502 Spring 2020
 * Version      1.0
 * Copyright    2020, All Rights Reserved
 *
 * Description
 *   This file controls the logging and backup
 *   features, and handles the signals.
 */

#include <fstream>  // file io
#include <sys/inotify.h> // inotify facility
#include <linux/limits.h> //max path size
#include <unistd.h> //read and write
#include <stdlib.h> // malloc
#include <signal.h> // signal handler functionality
#include <errno.h> // error handling
#include <string> // include strings
#include <string.h> // strcmp

#include "hw5.h"

using namespace std;

// simple signal handler sets a global variable
// to the value of the signal
void sigHandler(int num)
{
  theSig = num;
}

void dirMon(map<int, string>& argMap)
{
  // handle housekeeping matters
  fclose(stdout);
  fclose(stdin);
  fclose(stderr);

  // bind signals to signal handler
  struct sigaction sa;
  sa.sa_handler = sigHandler;
  sa.sa_flags = 0;

  sigaction(SIGINT, &sa, NULL);
  sigaction(SIGTERM, &sa, NULL);
  sigaction(SIGHUP, &sa, NULL);

  // open the file to log to
  ofstream fp;
  fp.open(argMap[LOGFILE].c_str());

  fp << "BEGIN LOGING" << endl;
  fp << "MY PROCESS ID IS " << getpid() << endl;

  // create the pid file
  if(ifstream("cs3377dirmond.pid"))
  {
    fp << "ERROR - PID FILE EXISTS, DAEMON ALREADY RUNNING" << endl;
    return;
  }
  ofstream pidFile ("cs3377dirmond.pid");
  pidFile << getpid() << endl;
  pidFile.close();


  // create the .versions folder
  string arg = "mkdir " + argMap[WATCHDIR] + "/.versions";
  fp << "CREATING .VERSIONS FOLDER" << endl;
  sys(arg.c_str());

  //define what the buffersize and inotify event are
  size_t bufsize = sizeof(struct inotify_event) + PATH_MAX + 1;
  struct inotify_event * event;

  //define a string of length buffer length
  char buffer [bufsize];

  //create the instance of inotify in the kernal
  int fd = inotify_init();
  
  //make sure that init was successful
  if ( fd < 0 )
  {
    fp << "ERROR INITIALIZING INOTIFY" << endl;
  }

  // add the watch for the given directiory
  int wd = inotify_add_watch( fd, argMap[WATCHDIR].c_str(), IN_MODIFY);

  // signal waiting for change
  //std::cout << "ready to wait for change" << std::endl;

  while(true)
  {
    // read and put the program into blocked until inotify is triggered
    int length = read ( fd, buffer, bufsize );

    // check to see if read returned with an error.  This means we were
    // interrupted by a signal
    if(length == -1 && errno == EINTR)
    {
      // the signal asked the program to stop cleanly, so
      // close all files, log the exit, and exit.
      if(theSig == SIGINT || theSig == SIGTERM)
      {
	fp << "EXITING DAEMON DUE TO SIGNAL" << endl;
	fp.close();
	inotify_rm_watch(fd, wd);
	close (fd);
	const char* commandToRemovePid = "rm -f cs3377dirmond.pid"; 
	sys(commandToRemovePid);
	return;
      }
      // the signal asked the program to reread the config file and
      // update anything necessary.  Here we update the logfile
      if(theSig == SIGHUP)
      {
	fp << "UPDATING CONFIG FILE" << endl;
	const char* result = parseConfigToMap(argMap[CONFIGFILE], argMap, true);
	if(result != NULL)
	{
	  fp << result << endl;
	  fp << "CLOSING DAEMON" << endl;
	  fp.close();
	  inotify_rm_watch(fd, wd);
	  close (fd);
	  const char* commandToRemovePid = "rm -f cs3377dirmond.pid"; 
	  sys(commandToRemovePid);
	  return;
	}
	fp.close();
	fp.open(argMap[LOGFILE].c_str(), ofstream::app);
      }
    }

    // handle anything we need to with the event.
    // e.g. if the verbose flag is set, then log the file change
    event = (struct inotify_event *) &buffer;
    const char* time = sys("date \"+%Y.%m.%d-%H.%M.%S\"");
    if(strcmp(argMap[VERBOSE].c_str(), "true") == 0)
    {
      fp << event->name << " was modified at time " << time <<  endl;
    }
    string arg = "cp " + argMap[WATCHDIR] + "/" + event->name + " " + argMap[WATCHDIR] + "/.versions/" + event->name + "." + time;
    sys(arg.c_str());
  }
}
