#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

// prototypes
void OnError(char const *errorMsg);
int GetTask();
void UpdateMarks(void);
int GetArrIndexByRegNo(char RegNo[]);
void DeleteMarks(void);

typedef struct
{
  char student_index[12]; // EG/XXXX/XXXX
  float assgnmt01_marks;  // 15%
  float assgnmt02_marks;  // 15%
  float project_marks;    // 20%
  float finalExam_marks;  // 50%
} student_marks;

student_marks empty_marks;

student_marks marksArr[100];

int main(int argc, char const *argv[])
{
  FILE *fd;
  fd = fopen("dataFile", "w");
  if (fd == NULL)
  {
    OnError("fopen dataFile error: ");
  }

  // Returning to user input on task completion forever.
  while (1)
  {
    // selecting the task based on user input - CLI for program operation
    switch (GetTask())
    {
    case 1: // insert
      InsertMarks();
      break;
    case 2: // update
      UpdateMarks();

      break;
    case 3: // delete
      DeleteMarks();
      break;

    default:
      OnError("GetTask error: ");
      break;
    }

    // writing marks changes to the file
    int ret_fwrite = fwrite(&marksArr, sizeof(student_marks), sizeof(marksArr) / sizeof(student_marks), fd);
    if (ret_fwrite < 0)
    {
      OnError("fwrite error: ");
    }
    else
    {
      printf("successfully written to dataFile\n");
    }
    fclose(fd);
  }
  return 0;
}

void OnError(char const *errorMsg)
{
  printf("Error No. %d\n", errno);
  perror(errorMsg);
  exit(1);
}

int GetTask()
{
  int input;
  printf("Enter the task to perform.\n");
  printf("[1] Insert \t [2] Update \t [3] Delete\n");
  scanf("%d", &input);
  if (input <= '3' && input >= '1')
  {
    return atoi(input);
  }
  return GetTask();
}

void InsertMarks(void)
{
  char regNo[12];
  // get student index
  printf("Enter student index (EG/XXXX/XXXX): ");
  gets(regNo);
  for (int i = 0; i < 100; i++)
  {
    if (marksArr[i].student_index == NULL)
      continue;
    strcpy(&marksArr[i].student_index, regNo);

    // get assgnmt01_marks
    printf("Enter assgnmt01_marks(%%): ");
    while (scanf("%f", &marksArr[i].assgnmt01_marks) != 1)
    {
      printf("Invalid assgnmt01_marks\n");
    }

    // get assgnmt02_marks
    printf("Enter assgnmt02_marks(%%): ");
    while (scanf("%f", &marksArr[i].assgnmt02_marks) != 1)
    {
      printf("Invalid assgnmt02_marks\n");
    }

    // get project_marks
    printf("Enter project_marks(%%): ");
    while (scanf("%f", &marksArr[i].project_marks) != 1)
    {
      printf("Invalid project_marks\n");
    }

    // get finalExam_marks
    printf("Enter finalExam_marks(%%): ");
    while (scanf("%f", &marksArr[i].finalExam_marks) != 1)
    {
      printf("Invalid finalExam_marks\n");
    }

    // Get whether to insert more or finish insertion branch
    printf("[default] Add more \t [q] Quit insert mode\n");
    if (getchar() == 'q' || getchar() == 'Q')
    {
      return;
    }
  }
}

void UpdateMarks(void)
{
  // Get whether to update or finish updates mode
  printf("[default] Continue Update \t [q] Quit update mode\n");
  if (getchar() == 'q' || getchar() == 'Q')
  {
    return;
  }

  // On continue update

  char regNo[12];
  // get student index to update
  printf("Enter student index (EG/XXXX/XXXX): ");
  gets(regNo);

  int upIndex = GetArrIndexByRegNo(regNo);
  if ((upIndex) == -1)
  {
    printf("student index not found. re-enter the student index\n");
    UpdateMarks();
  }

  // On found
  // get assgnmt01_marks
  printf("Enter assgnmt01_marks(%%): ");
  while (scanf("%f", &marksArr[upIndex].assgnmt01_marks) != 1)
  {
    printf("Invalid assgnmt01_marks\n");
  }

  // get assgnmt02_marks
  printf("Enter assgnmt02_marks(%%): ");
  while (scanf("%f", &marksArr[upIndex].assgnmt02_marks) != 1)
  {
    printf("Invalid assgnmt02_marks\n");
  }

  // get project_marks
  printf("Enter project_marks(%%): ");
  while (scanf("%f", &marksArr[upIndex].project_marks) != 1)
  {
    printf("Invalid project_marks\n");
  }

  // get finalExam_marks
  printf("Enter finalExam_marks(%%): ");
  while (scanf("%f", &marksArr[upIndex].finalExam_marks) != 1)
  {
    printf("Invalid finalExam_marks\n");
  }

  UpdateMarks();
}

// returns index if found.
// returns -1 if not found
int GetArrIndexByRegNo(char RegNo[])
{
  for (int i = 0; i < 100; i++)
  {
    if (marksArr[i].student_index == RegNo)
    {
      return i;
    }
  }

  // Not found
  return -1;
}

void DeleteMarks(void)
{
  // Get whether to delete or finish delete mode
  printf("[default] Continue Delete \t [q] Quit Delete Mode\n");
  if (getchar() == 'q' || getchar() == 'Q')
  {
    return;
  }

  // On continue delete

  char regNo[12];
  // get student index to delete
  printf("Enter student index (EG/XXXX/XXXX): ");
  gets(regNo);

  int delIndex = GetArrIndexByRegNo(regNo);
  if (delIndex == -1)
  {
    printf("student index not found. re-enter the student index\n");
    DeleteMarks();
  }

  // On found

  marksArr[delIndex] = empty_marks; // Delete the specified marks object

  DeleteMarks();
}