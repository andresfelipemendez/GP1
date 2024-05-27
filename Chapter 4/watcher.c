#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include <string.h>

int main() {
  char* buffer;
  if((buffer = _getcwd(NULL,0))==NULL)
  {
    perror("_getcwd error");
  }
  else {
    printf("path: %s \nLength: %zu\n", buffer, strlen(buffer));
    free(buffer);
  }
  return 0;
}
