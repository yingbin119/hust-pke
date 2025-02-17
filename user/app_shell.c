/*
 * This app starts a very simple shell and executes some simple commands.
 * The commands are stored in the hostfs_root/shellrc
 * The shell loads the file and executes the command line by line.                 
 */
#include "user_lib.h"
#include "string.h"
#include "util/types.h"
#include "util/string.h"
int main(int argc, char *argv[]) {
  wait(1);
  printu("\n======== HUST-PKE Shell Start ========\n\n");
  char command[128], para[128], cwd[128], para2[128];
  char command2[128] = "/bin/";
  char username[128] = "root";
  char password[128] = "123456";
  while (1){
    pwd(cwd);
    printu("[%s@localhost %s]$ ", username,cwd);
    scanu("%s %s", command, para);
    if (strcmp("q", command) == 0) // quit shell
    {
      break;
    }else if(strcmp("pwd", command) == 0){
      pwd(cwd);
      printu("%s\n", cwd);
    }else if(strcmp("cd", command) == 0){
      cd(para);
    }else{
      int pid = fork();
      if(pid == 0) {
        // strcat(para2, "RAMDISK0");
        // strcat(para2, cwd);
        // strcat(para2, para);
        strcat(command2, command);
        int ret = exec(command2, para);
        exit(0);
        return ret;
      }
      else
      {
        wait(pid);
      }
    }
  }
  exit(0);
  return 0;
}
