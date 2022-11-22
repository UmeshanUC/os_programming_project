#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <wait.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>

// prototypes
void OnError(char const *errorMsg);
void LoadData(void);

struct student_marks
{
  char student_index[12]; // EG/XXXX/XXXX
  float assgnmt01_marks;  // 15%
  float assgnmt02_marks;  // 15%
  float project_marks;    // 20%
  float finalExam_marks;  // 50%
};
struct student_marks marksArr[100];

int main(int argc, char const *argv[])
{
  pid_t PID_C1;
  pid_t PID_CC1;
  pid_t PID_CC2;
  pid_t PID_CC3;

  // Loading data from dataFile
  LoadData();

  // Creating a file token to use as key in shmget
  key_t ky = ftok("dataFile", 11);
  if (ky == -1)
  {
    OnError("ftok error: ");
  }

  // Requesting a shared memory segment
  int SMID = shmget(ky, 4096, IPC_CREAT | 0666);
  if (SMID == -1)
  {
    OnError("shmget error: ");
  }

  // Creating the child process C1
  PID_C1 = fork();
  if (PID_C1 < 0)
  {
    OnError("parent fork error: ");
  }
  else if (PID_C1 > 0) // P
  {

    waitpid(PID_C1, NULL, 0);
  }
  else if (PID_C1 == 0) // C1
  {
#pragma region // Creating the child process CC1
    PID_CC1 = fork();
    if (PID_CC1 < 0)
    {
      OnError("parent fork error: ");
    }
    else if (PID_CC1 > 0) // C1
    {

      waitpid(PID_CC1, NULL, 0);
    }
    else if (PID_CC1 == 0) // CC1
    {
      PID_CC1 = fork();
    }
#pragma endregion

#pragma region // Creating the child process CC2
    PID_CC2 = fork();
    if (PID_CC2 < 0)
    {
      OnError("parent fork error: ");
    }
    else if (PID_CC2 > 0) // C1
    {

      waitpid(PID_CC2, NULL, 0);
    }
    else if (PID_CC2 == 0) // CC1
    {
      PID_CC2 = fork();
    }
#pragma endregion

#pragma region // Creating the child process CC3
    PID_CC3 = fork();
    if (PID_CC3 < 0)
    {
      OnError("parent fork error: ");
    }
    else if (PID_CC3 > 0) // C1
    {

      waitpid(PID_CC3, NULL, 0);
    }
    else if (PID_CC1 == 0) // CC1
    {
      PID_CC3 = fork();
    }
#pragma endregion
  }
  return 0;
}

void OnError(char const *errorMsg)
{
  printf("Error No. %d\n", errno);
  perror(errorMsg);
  exit(1);
}

/// @brief Loading data to marksArr from the dataFile
void LoadData(void)
{
  int errNo;
  FILE *fdRead, *fdWrite;
  fdRead = fopen("dataFile", "r");
  if (fdRead == NULL)
  {
    if (errno == 2)
    {
      printf("dataFile not found\n");
    }
  }
  else
  {
    while (!feof(fdRead))
    {
      fread(marksArr, sizeof(struct student_marks), 100, fdRead);

      if ((errNo = ferror(fdRead)) > 0)
      {
        OnError("fread error: ");
      }
    }
    printf("dataFile loaded successfully.\n");

    // // for testing
    //  printf("%s\n", marksArr[0].student_index);
    //  printf("%f\n", marksArr[0].assgnmt01_marks);
    //  printf("%f\n", marksArr[0].assgnmt02_marks);
    //  printf("%f\n", marksArr[0].project_marks);
    //  printf("%f\n", marksArr[0].finalExam_marks);
    //  fclose(fdRead);
  }
}