/***
 * Name: 	Wenda Zhao
 * BlazerId: 	wenda
 * Project #: 	1
 * Comment: 	I used code from textbook (Adv Programming in the UNIX),
 * 		Figure 4.22, as the reference.
 */
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
#include<linux/limits.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>

// declare functions
typedef int MyFunc(const char *,const struct stat*,int);
static MyFunc myfunc;        // function for files
static int myftw(const char *,MyFunc *);
static int dopath(MyFunc *);

// define globle var
static char *fullpath;    // store files' name
static long nreg, ndir, nblk, nchr, nfifo, nslink, nsock, ntot;    // count files in different type
static char *filename;
static int *filesize;
// change:
static int slnk_flag = 0;

// macros in myfunc
#define FTW_F 1		// file
#define FTW_D 2		// directory
#define FTW_NS 3	// can't get stat
#define FTW_ND 4	// can't read

int main(int argc,char *argv[])
{
    char cur_path[80];
    if(argc>2 || argc<1)
    {
		printf("Usage:%s pathname\n",argv[0]+2);
		exit(EXIT_FAILURE);
    }
    if (argc==2)
    	myftw(argv[1], myfunc);
    if (argc==1){
	getcwd(cur_path, sizeof(cur_path));
	myftw(cur_path, myfunc);
    }
/*
    ntot = nreg + ndir + nblk + nchr + nfifo + nslink + nsock;
    if(0 == ntot)
    {
		ntot = 1;
    }

    printf("socket files    = %7ld, %5.2f%%\n",nsock,nsock*100.0/ntot);
    printf("link files      = %7ld, %5.2f%%\n",nslink,nslink*100.0/ntot);
    printf("regular files   = %7ld, %5.2f%%\n",nreg,nreg*100.0/ntot);
    printf("block files     = %7ld, %5.2f%%\n",nblk,nblk*100.0/ntot);
    printf("directory files = %7ld, %5.2f%%\n",ndir,ndir*100.0/ntot);
    printf("character files = %7ld, %5.2f%%\n",nchr,nchr*100.0/ntot);
    printf("FIFO files      = %7ld, %5.2f%%\n",nfifo,nfifo*100.0/ntot);
    printf("total files     = %7ld, %5.2f%%\n",ntot,ntot*100.0/ntot);
*/

    return 0;
}

// define allocate fullpath
static int myftw(const char* pathname,MyFunc *pmyfunc)
{
    int ret;

    // allocate fullpath
    fullpath = (char *)malloc(sizeof(char)*PATH_MAX);
    strcpy(fullpath,pathname);
    ret = dopath(myfunc);
    free(fullpath);

    return ret;
}

// dopath() is getting stat of the file
// give FTW_* macro value to (int type) in myfunc's last parameter
// modify the fullpath var
static int dopath(MyFunc *pmyfunc)
{
    int ret;
    struct stat statbuf;
    char *ptr;
    DIR *dp;
    struct dirent* dirp;

    if(-1 == lstat(fullpath,&statbuf))	// get stat error
    {
		ret = pmyfunc(fullpath,&statbuf,FTW_NS);
		return ret;
    }
    if(S_ISDIR(statbuf.st_mode) != 1)
    {
		ret = pmyfunc(fullpath,&statbuf,FTW_F);
		return ret;
    }

    // make directory ++
    if(0 != (ret=pmyfunc(fullpath,&statbuf,FTW_D)))
		return ret;

    // if it's a directory, then open it
    if(-1 == chdir(fullpath))
    {
		printf("%s[chdir]%s\n",fullpath,strerror(errno));
		ret == -1;
		return ret;
    }

    // open current directory
    if(NULL == (dp=opendir(".")))
    {
//		printf("\n");
//		printf("%s:\n", dirp->d_name);
		ret = pmyfunc(fullpath,&statbuf,FTW_ND);
		return ret;
    }

    // readdir returns NULL on error or the end of dir
    while(NULL != (dirp=readdir(dp)))
    {
		// omit.and..file(dot) --"."piont current dir/".."point parent dir
		if(0==strcmp(dirp->d_name,".") || 0==strcmp(dirp->d_name,".."))
			continue;
		memset(fullpath,0,PATH_MAX);
		strcpy(fullpath,dirp->d_name);
		printf("%s", dirp->d_name);
//change
		if (slnk_flag == 1){
			printf("\n***symbolic link detected:%s\n", dirp->d_name);
		}

		if(0 != (ret=dopath(myfunc)))    // return
			break;
    }
    chdir("..");    // set current dir as previous dir
    // make sure closed file
    if(-1 == closedir(dp))
    {
		printf("Can't close %s\nError:%s",fullpath,strerror(errno));
    }
    printf("\n");
    return ret;
}
static int myfunc(const char * pathname,const struct stat * statptr,int type)
{
    switch(type)
    {
		case FTW_F:
		printf("\t");
		switch(statptr->st_mode & S_IFMT)
		{
//			case S_IFSOCK:    nsock++;    break;
			case S_IFLNK:
				slnk_flag = 1;
				break;
//			case S_IFREG:    nreg++;    break;
//			case S_IFBLK:    nblk++;    break;
//			case S_IFCHR:    nchr++;    break;
//			case S_IFIFO:    nfifo++;    break;
			case S_IFDIR:
				printf("Error:Here should not be a directory %s!\n\nError:%s\n",pathname,strerror(errno));
				break;
		}
		break;
		case FTW_D:
		printf(":\n");    break;
		case FTW_ND:
		printf("Can't open directory %s\nError:%s\n",pathname,strerror(errno));
		break;
		case FTW_NS:
		printf("Can't open file %s\nError:%s\n",pathname,strerror(errno));
		break;
    }
    return 0;
}
