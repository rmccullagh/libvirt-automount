#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char **argv)
{
  if(argc != 2)
  {
    fprintf(stderr, "this command needs an argument of <guestname>\n");
    return 1;
  }

  int fd;
  int pid;

  pid = fork();

  if(pid > 0)
  {
    _exit(0);
  }
  else
  {
    setsid();
    chdir("/");
    fd = open("/dev/null", O_RDWR, 0);
    dup2(fd, STDIN_FILENO);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
  }
 
  /* Now do the real work */

  execl("/etc/libvirt/hooks/qemu-start-wrapped", "qemu-start-wrapped", argv[1], (char *)NULL);
  fprintf(stderr, "failed to start process");

  return 0;
}

