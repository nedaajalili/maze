#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100
const int xoff[4]={0,1,0,-1};
const int yoff[4]={-1,0,1,0};

FILE *fileopen(char *, char *);
void *utilMalloc(int);
int move_r_one(char **,int,int,int,int);
void display(char **,int);
int move_r_all(char **,int,int,int,int,int,int);
int move_r_best(char **,char **,int,int,int,int,int,int);



int main(int argc,char *argv[])
{
    FILE *fp;
    int nr,nc,i,j,r,c,res;
    char line[MAX];
    char **mazeCurr;
    char **mazeBest;
    if(argc!=2)
    {
        printf("error in passing C.L arguments!");
        exit(0);
    }
    fp=fileopen(argv[1],"r");
    fgets(line,MAX,fp);
    sscanf(line,"%d %d",&nr,&nc);
    mazeCurr=(char **)utilMalloc(nr*sizeof(char *));
    mazeBest=(char **)utilMalloc(nr*sizeof(char *));
    for(i=0; i<nr; i++)
    {
        fgets(line,MAX,fp);
        mazeCurr[i]=strdup(line);
        mazeBest[i]=strdup(line);
        for(j=0; j<nc; j++)
        {
            if(mazeCurr[i][j]=='@')
            {
                r=i;
                c=j;
            }
        }
    }

    mazeCurr[r][c]=' ';
    res=move_r_one(mazeCurr,nr,nc,r,c);
    if(res==1){
        mazeCurr[r][c]='@';
        display(mazeCurr,nr);
    }else{
        printf("no solution!\n");
    }


    for(i=0;i<nr;i++){
        for(j=0;j<nc;j++){
            if(mazeCurr[i][j]=='$' || mazeCurr[i][j]=='.'){
                mazeCurr[i][j]=' ';
            }
        }
    }

    printf("all solutions:\n");
    mazeCurr[r][c]=' ';
    res=move_r_all(mazeCurr,nr,nc,r,c,r,c);

    printf("Best solution:\n");
    mazeCurr[r][c]=' ';
    res=move_r_best(mazeCurr,mazeBest,0,nr*nc,nr,nc,r,c);
    if(res>0){
        mazeCurr[r][c]='@';
        display(mazeCurr,nr);
    }else{
        printf("no solution!");
    }

    printf("%d %d",r,c);
    return 0;
}
int move_r_one(char **mazeCurr,int nr,int nc,int row,int col){
    int i,r,c;
    if(mazeCurr[row][col]=='#'){
        return 1;
    }
    if(mazeCurr[row][col]!=' '){
        return 0;
    }
    mazeCurr[row][col]='$';
    for(i=0;i<4;i++){
        r=row+xoff[i];
        c=col+yoff[i];
        if(r>=0 && c>=0 && r<nr && c<nc){
            if(move_r_one(mazeCurr,nr,nc,r,c))
                return 1;
        }
    }
    mazeCurr[row][col]='.';
    return 0;
}

int move_r_all(char **mazeCurr,int nr,int nc,int row,int col,int row0,int col0){
    int k,r,c,numsol=0;
    if(mazeCurr[row][col]=='#'){
        numsol++;
        printf("displaying solution number %d:\n",numsol);//it always prints 1
        mazeCurr[row0][col0]='@';
        display(mazeCurr,nr);
        mazeCurr[row0][col0]=' ';
        return 1;
    }
    if(mazeCurr[row][col]!=' '){
        return 0;
    }
    mazeCurr[row][col]='$';
    for(k=0;k<4;k++){
        r=row+xoff[k];
        c=col+yoff[k];
        if(r>=0 && c>=0 && r<nr && c<nc){
            move_r_all(mazeCurr,nr,nc,r,c,row0,col0);
        }
    }
    mazeCurr[row][col]=' ';
    return 0;
}
int move_r_best(char **mazeCurr,char **mazeBest,int currstep,int beststep,int nr,int nc,int row,int col){
    int i,j,k,r,c;
    if(currstep>=beststep){
        return beststep;
    }
    if(mazeCurr[row][col]=='#'){
        if(currstep<beststep){
            beststep=currstep;
            for(i=0;i<nr;i++){
                for(j=0;j<nc;j++){
                    mazeBest[i][j]=mazeCurr[i][j];
                }
            }
        }
        return beststep;
    }
    if(mazeCurr[row][col]!=' '){
        return beststep;
    }
    mazeCurr[row][col]='$';
    for(k=0;k<4;k++){
        r=row+xoff[k];
        c=col+yoff[k];
        if(r>=0 && c>=0 && r<nr && c<nc){
            beststep=move_r_best(mazeCurr,mazeBest,currstep+1,beststep,nr,nc,r,c);
        }
    }
    mazeCurr[row][col]=' ';
    return beststep;

}


FILE *fileopen(char *fn, char *mode)
{
    FILE *fp;
    fp=fopen(fn,mode);
    if(fp==NULL)
    {
        printf("error in file opening!");
        exit(0);
    }
    return fp;
}
void *utilMalloc(int n)
{
    void *ptr;
    ptr=malloc(n);
    if(ptr==NULL)
    {
        printf("error in allocation!");
        exit(0);
    }
    return ptr;
}
void display(char **mazeCurr,int nr){
    int i;
    for(i=0;i<nr;i++){
        printf("%s",mazeCurr[i]);
    }
}

