/* Project headers */
#include "utils.h"
#include "sha256.h"

/* stdlib headers */
#include <stdio.h>
#include <unistd.h>
#include <string.h>

/* Consts */
#define BUFF_SIZE       1024*1024   /* Max size of the input buffer */

/* Entry point */
int main(void) {
  char buff[BUFF_SIZE];     /* Max 1MiB file */
  u64 i;                    /* For iteration */
  read(STDIN_FILENO, buff, BUFF_SIZE);
  buff[strlen(buff)-1] = '\0';
  hash_t hashed = sha256(buff);
  for (i = 0; i < 8; i++)
    printf("%08x", _big_endian32(hashed.value[i]));
  printf("\n");
  return 0;
}
