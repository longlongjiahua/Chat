#include "net.h"

void diep(char *s)
{
  perror(s);
  exit(1);
}
