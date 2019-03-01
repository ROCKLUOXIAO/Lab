#include<unistd.h>
#include<sys/stat.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>
#include<time.h>
#include<pwd.h>
#define max 100
void getInfo(struct stat sb){
    printf("-");
    if(sb.st_mode& S_IRUSR){
        printf("r");
    }
    else {
        printf("-");
    }
    if (sb.st_mode & S_IWUSR) {
        printf("w");
    } else {
        printf("-");
    }
    if (sb.st_mode & S_IXUSR) {
        printf("x");
    } else {
        printf("-");
    }
    if (sb.st_mode & S_IRGRP) {
        printf("r");
    } else {
        printf("-");
    }
    if (sb.st_mode & S_IWGRP) {
        printf("w");
    } else {
        printf("-");
    }
    if (sb.st_mode & S_IXGRP) {
        printf("x");
    } else {
        printf("-");
    }
    if (sb.st_mode & S_IROTH) {
        printf("r");
    } else {
        printf("-");
    }
    if (sb.st_mode & S_IWOTH) {
        printf("w");
    } else {
        printf("-");
    }
    if (sb.st_mode & S_IXOTH) {
        printf("x\t");
    } else {
        printf("-\t");
        return ;
    }
}
void printdir(char *dir, int depth){
    DIR *dp;//pointer to dir;
    struct dirent *entry;
    struct stat statbuf;
    if((dp=opendir(dir))==NULL){//failed to open the current dir
        printf("failed to open the directory\n");
        exit(0);
    }
    chdir(dir);//change it as the current dir
    while((entry=readdir(dp))!=NULL){//read the file is not NULL
        lstat(entry->d_name,&statbuf);
        if(S_ISDIR(statbuf.st_mode)){
            if(strcmp(entry->d_name,"..")==0||strcmp(entry->d_name,".")==0)//if the current dir or the father dir
            continue;
            getInfo(statbuf);//
            printf("%d\t",depth);//print the depth
            printf("%s\t",getpwuid(statbuf.st_uid)->pw_name);//name of file's user 
            printf("%s\t",getpwuid(statbuf.st_gid)->pw_name);//name of the group of file's user
            printf("%ld\t",statbuf.st_size);//length of file
            printf("%s\t",strtok(ctime(&statbuf.st_mtime),"\n"));//the last time to modify the file
            printf("%s\n",entry->d_name);
            printdir(entry->d_name,depth+4);
        }
        else{//is not the dir ,is a file
            getInfo(statbuf);//
            printf("%d\t",depth);//depth
            printf("%s\t",getpwuid(statbuf.st_uid)->pw_name);//name of file's user 
            printf("%s\t",getpwuid(statbuf.st_gid)->pw_name);//name of the group of file's user
            printf("%ld\t",statbuf.st_size);//length of file
            printf("%s\t",strtok(ctime(&statbuf.st_mtime),"\n"));//the last time to modify the file
            printf("%s\n",entry->d_name);
        }
    }
    chdir("..");//return the father dir
    closedir(dp);//close the dir
}
int main(){
    char dir[max];
    printf("input the directory\n");
    scanf("%s",dir);
    printdir(dir,1);
    return 0;
}