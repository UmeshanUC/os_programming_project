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
#include <string.h>

#define MIN 1
#define MAX 2
#define AVG 3
#define ABOVE10NUM 4

// prototypes
void OnError(char const *errorMsg);
void LoadData(void);
float CalAss02_Min(void);
float CalAss02_Max(void);
float CalAss02_Avg(void);
int CalAss02_Above10_Num(void);
void SendViaSM(float value, int SMID, int type);

struct student_marks
{
  char student_index[12]; // EG/XXXX/XXXX
  float assgnmt01_marks;  // 15%
  float assgnmt02_marks;  // 15%
  float project_marks;    // 20%
  float finalExam_marks;  // 50%
};

// Data structure used to transfer calculation results
struct results
{
  float min;        // 1
  float max;        // 2
  float avg;        // 3
  float above10Num; // 4
};

struct student_marks null_marks = {"NULL", 0.0, 0.0, 0.0, 0.0};

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
    struct results receivedResults;
    struct results *ptr;
    ptr = (struct results *)shmat(SMID, NULL, SHM_W);
    if (ptr == (void *)-1)
    {
      OnError("parent shmat error: ");
    }

    waitpid(PID_C1, NULL, 0);
    waitpid(PID_CC1, NULL, 0);
    waitpid(PID_CC2, NULL, 0);
    waitpid(PID_CC3, NULL, 0);
    shmdt(ptr);

    // Outputs
    printf("Min value: %f\n", receivedResults.min);
    printf("Max value: %f\n", receivedResults.max);
    printf("Avg value: %f\n", receivedResults.avg);
    printf("Number of students above 10%%: %f\n", receivedResults.above10Num);
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
      float max = CalAss02_Max();
      SendViaSM(max, SMID, MAX);
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
    else if (PID_CC2 == 0) // CC2
    {
      float avg = CalAss02_Avg();
      SendViaSM(avg, SMID, AVG);
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
    else if (PID_CC1 == 0) // CC3
    {
      int above10Num = CalAss02_Above10_Num();
      SendViaSM(above10Num, SMID, ABOVE10NUM);
    }
#pragma endregion

    float min = CalAss02_Min();
    SendViaSM(min, SMID, MIN);
  }

  shmctl(SMID, IPC_RMID, NULL);
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

/// @brief Calculating Min student record for Assignment02 marks
float CalAss02_Min(void)
{
  float min = 100; // current min
  int indexOfMin;  // Index of student record with min assign02 marks
  for (int i = 0; i < 100; i++)
  {
    // assign as new min if the current marks is lesser than current min
    if (min > marksArr[i].assgnmt02_marks)
    {
      min = marksArr[i].assgnmt02_marks;
      indexOfMin = i;
    }
  }
  return min;
}

/// @brief Calculating Max student record for Assignment02 marks
float CalAss02_Max(void)
{
  float max = 100; // current max
  int indexOfMax;  // Index of student record with max assign02 marks
  for (int i = 0; i < 100; i++)
  {
    // assign as new max if the current marks is higher than current max
    if (max < marksArr[i].assgnmt02_marks)
    {
      max = marksArr[i].assgnmt02_marks;
      indexOfMax = i;
    }
  }
  return max;
}

/// @brief Calculating Average of Assignment02 marks
float CalAss02_Avg(void)
{
  float sum;
  int num;
  for (int i = 0; i < 100; i++)
  {
    if (!strcmp(marksArr[i].student_index, null_marks.student_index))
    {
      num++;
    }
    sum += marksArr[i].assgnmt02_marks;
  }
  return sum / num;
}

/// @brief Calculating Number of students above 10% for Assignment02 marks
int CalAss02_Above10_Num(void)
{
  int num;
  for (int i = 0; i < 100; i++)
  {
    if (marksArr[i].assgnmt02_marks > (float)10)
    {
      num++;
    }
  }
  return num;
}

void SendViaSM(float value, int SMID, int type)
{
  struct results *ptr;
  ptr = (struct results *)shmat(SMID, NULL, SHM_W);
  if (ptr == (struct results *)-1)
  {
    OnError("shmat error: ");
  }
  switch (type)
  {
  case MIN:
    ptr->min = value;
    break;
  case MAX:
    ptr->max = value;
    break;
  case AVG:
    ptr->avg = value;
    break;
  case ABOVE10NUM:
    ptr->above10Num = value;
    break;

  default:
    break;
  }
  shmdt(ptr);
}
