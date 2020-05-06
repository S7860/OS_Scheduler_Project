#include<sys/resource.h>
#include<sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include<stdio.h>


//during compilation you can increase ur address space
//tell linker to add more to the stack

int main()
{
  //occur when the program tries to write/read outside the memory allocated
  //for it or when writing memory which can only be read

  int size = rand() % 500;
  int matrix1[size][size];
  int matrix2[size][size];
  int res_matrix[size][size];
  int r, c;

  // creates a random size x size matrix
  srand(time(NULL));
  for(r = 0; r < size; r++)
  {
    for(c = 0; c < size; c++) {
      matrix1[r][c] = rand() % 4;
      matrix2[r][c] = rand() % 4;
      // printf("%d  ", matrix1[r][c]);
    }
    // printf("\n");
  }

  // printf("\n\n");
  for(r = 0; r < size; r++)
  {
    for(c = 0; c < size; c++) {
      // printf("%d  ", matrix2[r][c]);
    }
    // printf("\n");
  }
  // printf("\n\n");



  printf("[main] Hi There! My PID is %d\n", getpid());

  int status;
  struct rusage usage;

  time_t begin = time(NULL);
  pid_t pid = fork();

  if(pid == 0)
  {
    printf("[child] I'm alive! My PID is %d and PPID is %d\n", getpid(), getppid());
    printf("[child] I'm gonna multiply your matrix papa...\n");

    int i, j, k;
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            res_matrix[i][j] = 0;
            for (k = 0; k < size; k++)
                res_matrix[i][j] += matrix1[i][k] * matrix2[k][j];
        }
    }

    // printf("\n\n");
    // for(r = 0; r < size; r++)
    // {
    //   for(c = 0; c < size; c++) {
    //     printf("%d  ", res_matrix[r][c]);
    //   }
    //   printf("\n");
    // }

    exit(0);
  }
  else if(pid > 0)
  {
    printf("[main] Forked to make a child with pid %d\n", pid);
    printf("[main] I'll be waiting for the child termination...\n\n");

    int c;
    FILE *out;
    out = fopen("info_dynamic.csv", "a+");

    // fclose(out);
    FILE *file1;
    FILE *file2;
    FILE *file3;

    char fileName1[20];
    char fileName2[20];
    char fileName3[20];
    snprintf(fileName1, 20, "/proc/%d/stat", pid);
    snprintf(fileName2, 20, "/proc/%d/sched", pid);
    snprintf(fileName3, 20, "/proc/%d/io", pid);

    for(size_t num = 0; num < 100; num++) {
      if ( file2 = fopen(fileName2, "r") ) {
        char prev = '*';
        int noOfLines = 0;
        while ((c = getc(file2)) != EOF) {
          if (c == '\n') noOfLines++;
          if (noOfLines == 26) break;
          if (prev == ':')
            fprintf(out, " ");
          if (c != ' ' && c != '\t')
            fprintf(out, "%c", c);
          prev = c;
        }
        fclose(file2);
      }
      else printf("File Not Found");

      if ( file3 = fopen(fileName3, "r") ) {
          while ((c = getc(file3)) != EOF)
              fprintf(out, "%c", c);
          fclose(file3);
      }
      else printf("File Not Found");
    }

    wait4(pid, &status, 0, &usage);
    time_t end = time(NULL);
    double time_spent = (double)(end - begin);

    printf("[main] Child has finished termination!\n");

    fprintf(out, "Turnaround Time: %f", time_spent);
    fclose(out);

    printf("[main] Child has finished termination!\n");

    FILE *fp;
    fp = fopen("info.csv", "a+");
    fprintf(fp, "%ld.%ld,%ld.%ld,%ld,%ld,%ld,%ld,%ld,%ld\n", usage.ru_utime.tv_sec, usage.ru_utime.tv_usec, usage.ru_stime.tv_sec, usage.ru_stime.tv_usec, usage.ru_minflt, usage.ru_majflt, usage.ru_inblock, usage.ru_oublock, usage.ru_nvcsw, usage.ru_nivcsw);
    fclose(fp);
  }
}
