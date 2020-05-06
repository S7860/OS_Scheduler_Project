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

  printf("[main] Hi There! My PID is %d\n", getpid());

  int status;
  struct rusage usage;

  time_t begin = time(NULL);
  pid_t pid = fork();

  if(pid == 0)
  {
    printf("[child] I'm alive! My PID is %d and PPID is %d\n", getpid(), getppid());
    printf("[child] I'm gonna read-write files papa...\n");

    void quicksort(int array[25],int first,int last){
       int i, j, pivot, temp;

       if(first<last){
          pivot=first;
          i=first;
          j=last;

          while(i<j){
             while(array[i]<=array[pivot]&&i<last)
                i++;
             while(array[j]>array[pivot])
                j--;
             if(i<j){
                temp=array[i];
                array[i]=array[j];
                array[j]=temp;
             }
          }

          temp=array[pivot];
          array[pivot]=array[j];
          array[j]=temp;
          quicksort(array,first,j-1);
          quicksort(array,j+1,last);

       }
    }

    srand(time(0));
    int size = rand() % 5000000;
    int array[size];
    for (int i = 0; i < size; i++) {
      array[i] = rand() % 9999;
    }

    quicksort(array, 0, size-1);

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

    FILE *fp;
    fp = fopen("info.csv", "a+");
    fprintf(fp, "%ld.%ld,%ld.%ld,%ld,%ld,%ld,%ld,%ld,%ld,%f\n", usage.ru_utime.tv_sec, usage.ru_utime.tv_usec, usage.ru_stime.tv_sec, usage.ru_stime.tv_usec, usage.ru_minflt, usage.ru_majflt, usage.ru_inblock, usage.ru_oublock, usage.ru_nvcsw, usage.ru_nivcsw, time_spent);
    fclose(fp);
  }
}
