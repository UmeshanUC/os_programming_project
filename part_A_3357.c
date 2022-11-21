#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

// prototypes
void OnError(char const *errorMsg);

typedef struct
{
  char student_index[12]; // EG/XXXX/XXXX
  float assgnmt01_marks;  // 15%
  float assgnmt02_marks;  // 15%
  float project_marks;    // 20%
  float finalExam_marks;  // 50%
} student_marks;

int main(int argc, char const *argv[])
{
  FILE *fd;
  fd = fopen("dataFile", "w");
  int ret;

  if (fd == NULL)
  {
    OnError()
  }
  else
  {
    char name[20];
    int age;

    printf("Name: ");
    scanf("%s", name);

    printf("Age: ");
    scanf("%d", &age);

    ret = fwrite(name, sizeof(char), sizeof(name) / sizeof(name[1]), fd);

    if (ret < 0)
    {
      perror("fprint doc1.txt:");
      printf("Error No: %d\n", errno);
      exit(1);
    }
    else
    {
      printf("successfully written to file\n");
    }
  }
  fclose(fd);
  return 0;
}

void OnError(char const *errorMsg)
{
  printf("Error No. %d\n", errno);
  perror(errorMsg);
  exit(1);
}