#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<errno.h>
#include<string.h>

#define MAX_BUF 1024
void highlight_pattern(char *line,const char *pattern)
{
const char *start=line;
const char *match_start;
int pat_len=strlen(pattern);

while((match_start=strstr(start,pattern))!=NULL)
{
fwrite(start,1,match_start-start,stdout);
printf("\033[1;31m");                           // Start red color
        fwrite(match_start, 1, pat_len, stdout);    // Print match in red
        printf("\033[0m");                              // Reset color
        start = match_start + pat_len;              // Move to next part
    }
    printf("%s", start);  // Print remaining text
}
int main()
{
int ch;
while(1)
{
printf("\nMENU:\n1.Simulate cp command.\n2.Simulate grep command.\n3.exit\n");
printf("Enter your choice:");
scanf("%d",&ch);

switch(ch)
{
case 1:
{
char source[MAX_BUF],dest[MAX_BUF];
printf("\nEnter source filename:");
scanf("%s",source);
printf("\nEnter destination filename:");
scanf("%s",dest);

pid_t pid=fork();
if(pid==0) //child process
{
int src_fd=open(source,O_RDONLY);
int dest_fd=open(dest,O_WRONLY | O_CREAT | O_TRUNC,0644);
if(src_fd==-1|| dest_fd==-1)
{
perror("Error");
exit(1);
}
char buffer[MAX_BUF];
ssize_t bytes_reads;
while((bytes_reads=read(src_fd,buffer,sizeof(buffer)))>0)
{
write(dest_fd,buffer,bytes_reads);
}
close(src_fd);
close(dest_fd);
printf("file copied successfully");
exit(0);
}
else
{
wait(NULL);//parent waits
}
break;
}

case 2://simulate grep command
{
char pattern[MAX_BUF],filename[MAX_BUF];
printf("\nEnter pattern to search:");
scanf("%s",pattern);
printf("\nEnter filename:");
scanf("%s",filename);

FILE *file=fopen(filename,"r");
if(!file)
{
perror("Error in opening file!!!");
exit(1);
}
char line[MAX_BUF];
while (fgets(line,MAX_BUF,file))
highlight_pattern(line,pattern);

fclose(file);
break;
}

case 3:
exit(0);

default:
printf("\nInvalid choice.");
}
}
return 0;
}