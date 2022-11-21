#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

// prototypes
void OnError(char const *errorMsg);
int GetTask();
void InsertMarks(void);
void UpdateMarks(void);
void DeleteMarks(void);
int GetArrIndexByRegNo(char RegNo[]);
void FlushInputStream(void);

struct student_marks
{
  char student_index[12]; // EG/XXXX/XXXX
  float assgnmt01_marks;  // 15%
  float assgnmt02_marks;  // 15%
  float project_marks;    // 20%
  float finalExam_marks;  // 50%
};

struct student_marks null_marks = {"NULL", 0.0, 0.0, 0.0, 0.0};

struct student_marks marksArr[100];

int main(int argc, char const *argv[])
{

  // Initialize all marks records to null_marks
  for (int i = 0; i < 100; i++)
  {
    marksArr[i] = null_marks;
  }

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
    int ret_fwrite = fwrite(&marksArr, sizeof(struct student_marks), sizeof(marksArr) / sizeof(struct student_marks), fd);
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
  char input;
  printf("Enter the task to perform.\n");
  printf("[1] Insert \t [2] Update \t [3] Delete\n");
  input = getchar();
  FlushInputStream();
  if (input <= '3' && input >= '1')
  {
    return atoi(&input);
  }
  int out = GetTask();
  return out;
}

void InsertMarks(void)
{
  char regNo[12];
  // get student index
  printf("Enter student index (EG/XXXX/XXXX): ");
  scanf("%s", regNo);
  FlushInputStream();
  for (int i = 0; i < 100; i++)
  {
    // Skip to target the next array element in marksArr if it's not empty
    if (strcmp(marksArr[i].student_index, null_marks.student_index))
    {
      printf("%s\n", marksArr[i].student_index);
      continue;
    }

    strcpy(marksArr[i].student_index, regNo);

    // get assgnmt01_marks
    printf("Enter assgnmt01_marks(%%): ");
    while (scanf(" %f\n", &marksArr[i].assgnmt01_marks) < 1)
    {
      printf("Invalid assgnmt01_marks\n");
      FlushInputStream();
    }

    // get assgnmt02_marks
    printf("Enter assgnmt02_marks(%%): ");
    while (scanf(" %f\n", &marksArr[i].assgnmt02_marks) < 1)
    {
      printf("Invalid assgnmt02_marks\n");
      FlushInputStream();
    }

    // get project_marks
    printf("Enter project_marks(%%): ");
    while (scanf(" %f\n", &marksArr[i].project_marks) < 1)
    {
      printf("Invalid project_marks\n");
      FlushInputStream();
      ;
    }

    // get finalExam_marks
    printf("Enter finalExam_marks(%%): ");
    while (scanf(" %f\n", &marksArr[i].finalExam_marks) < 1)
    {
      printf("Invalid finalExam_marks\n");
      FlushInputStream();
    }

    // Get whether to insert more or finish insertion branch
    printf("[default] Add more \t [q] Quit insert mode\n");
    FlushInputStream();
    char ch = getchar();
    if (ch == 'q' || ch == 'Q')
    {
      return;
    }
    else
    {
      InsertMarks();
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
  scanf("%s", regNo);
  FlushInputStream();

  int upIndex = GetArrIndexByRegNo(regNo);
  if ((upIndex) == -1)
  {
    printf("student index not found. re-enter the student index\n");
    UpdateMarks();
  }

  // On found
  // get assgnmt01_marks
  printf("Enter assgnmt01_marks(%%): ");
  while (scanf(" %f\n", &marksArr[upIndex].assgnmt01_marks) < 1)
  {
    printf("Invalid assgnmt01_marks\n");
    FlushInputStream();
  }

  // get assgnmt02_marks
  printf("Enter assgnmt02_marks(%%): ");
  while (scanf(" %f\n", &marksArr[upIndex].assgnmt02_marks) < 1)
  {
    printf("Invalid assgnmt02_marks\n");
    FlushInputStream();
  }

  // get project_marks
  printf("Enter project_marks(%%): ");
  while (scanf(" %f\n", &marksArr[upIndex].project_marks) < 1)
  {
    printf("Invalid project_marks\n");
    FlushInputStream();
  }

  // get finalExam_marks
  printf("Enter finalExam_marks(%%): ");
  while (scanf(" %f\n", &marksArr[upIndex].finalExam_marks) < 1)
  {
    printf("Invalid finalExam_marks\n");
    FlushInputStream();
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
  scanf("%s", regNo);
  FlushInputStream();

  int delIndex = GetArrIndexByRegNo(regNo);
  if (delIndex == -1)
  {
    printf("student index not found. re-enter the student index\n");
    DeleteMarks();
  }

  // On found

  marksArr[delIndex] = null_marks; // Delete the specified marks object

  DeleteMarks();
}

void FlushInputStream(void)
{
  char ch;
  while ((ch = getchar()) != '\n' && ch != EOF)
    ;
}