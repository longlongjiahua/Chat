#include "net.h"

#include <stdlib.h>
#include <stdio.h>

void diep(char *s)
{
  perror(s);
  exit(1);
}
