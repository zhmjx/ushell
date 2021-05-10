#include <pty.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/select.h>
#include <string.h>

int pty_main(int argc, char *argv[])
{
  int master;
  pid_t pid = forkpty(&master, NULL, NULL, NULL);  // opentty + login_tty + fork
  
  if (pid < 0) {
    return 1; // fork with pseudo terminal failed
  }
  
  else if (pid == 0) {   // child
    char **args = &(argv[2]);// argument after "ushell -p"
    execvp(argv[2], args);  // run the given proc in pty
  }
  
  else {   // parent
    /*strncpy(argv[0], "ps", strlen(argv[0]));//hide cmd
    if (argc > 1)
    {
      int ii;
      for (ii=1;ii<argc;ii++)
      strncpy(argv[ii], "", strlen(argv[ii]));
    }*/

    struct termios tios;
    tcgetattr(master, &tios);
    tios.c_lflag &= ~(ECHO | ECHONL);
    tcsetattr(master, TCSAFLUSH, &tios);

    while(1) {
      fd_set read_fd, write_fd, err_fd;

      FD_ZERO(&read_fd);
      FD_ZERO(&write_fd);
      FD_ZERO(&err_fd);
      FD_SET(master, &read_fd);
      FD_SET(STDIN_FILENO, &read_fd);

      select(master+1, &read_fd, &write_fd, &err_fd, NULL);

      if (FD_ISSET(master, &read_fd))
      {
        char ch;
        int c;
        if (c = read(master, &ch, 1) != -1)  // read from program
          write(STDOUT_FILENO, &ch, c);  // write to tty
        else
          break;  // exit when end of communication channel with program
      }

      if (FD_ISSET(STDIN_FILENO, &read_fd))
      {
        char ch;
        int c = read(STDIN_FILENO, &ch, 1);   // read from tty
        write(master, &ch, c);        // write to program
      }
    }
  }
  return 0;
}

