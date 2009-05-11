#include<malloc.h>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>

#define fillincrement 0.01

#define FREE_ARG char*
#define NR_END 1

float **topo;
int lattice_size_x,lattice_size_y,*iup,*idown,*jup,*jdown;

int *ivector(nl,nh)
long nh,nl;
/* allocate an int vector with subscript range v[nl..nh] */
{
        int *v;

        v=(int *)malloc((unsigned int) ((nh-nl+1+NR_END)*sizeof(int)));
        return v-nl+NR_END;
}

float **matrix(long nrl, long nrh, long ncl, long nch)
/* allocate a float matrix with subscript range m[nrl..nrh][ncl..nch] */
{
	long i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
	float **m;

	m=(float **) malloc((size_t)((nrow+NR_END)*sizeof(float*)));
	m += NR_END;
	m -= nrl;

	m[nrl]=(float *) malloc((size_t)((nrow*ncol+NR_END)*sizeof(float)));
	m[nrl] += NR_END;
	m[nrl] -= ncl;

	for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;

	return m;
}

void setupgridneighbors()
{    int i,j;

     idown=ivector(1,lattice_size_x);
     iup=ivector(1,lattice_size_x);
     jup=ivector(1,lattice_size_y);
     jdown=ivector(1,lattice_size_y);
     for (i=1;i<=lattice_size_x;i++)
      {idown[i]=i-1;
       iup[i]=i+1;}
     idown[1]=1;
     iup[lattice_size_x]=lattice_size_x;
     for (j=1;j<=lattice_size_y;j++)
      {jdown[j]=j-1;
       jup[j]=j+1;}
     jdown[1]=1;
     jup[lattice_size_y]=lattice_size_y;
}

void fillinpitsandflats(i,j)
int i,j;
{   float min;

    min=topo[i][j];
    if (topo[iup[i]][j]<min) min=topo[iup[i]][j];
    if (topo[idown[i]][j]<min) min=topo[idown[i]][j];
    if (topo[i][jup[j]]<min) min=topo[i][jup[j]];
    if (topo[i][jdown[j]]<min) min=topo[i][jdown[j]];
    if (topo[iup[i]][jup[j]]<min) min=topo[iup[i]][jup[j]];
    if (topo[idown[i]][jup[j]]<min) min=topo[idown[i]][jup[j]];
    if (topo[idown[i]][jdown[j]]<min) min=topo[idown[i]][jdown[j]];
    if (topo[iup[i]][jdown[j]]<min) min=topo[iup[i]][jdown[j]];
    if ((topo[i][j]<=min)&&(i>1)&&(j>1)&&(i<lattice_size_x)&&(j<lattice_size_y))
     {topo[i][j]=min+fillincrement;
      fillinpitsandflats(i,j);
      fillinpitsandflats(iup[i],j);
      fillinpitsandflats(idown[i],j);
      fillinpitsandflats(i,jup[j]);
      fillinpitsandflats(i,jdown[j]);
      fillinpitsandflats(iup[i],jup[j]);
      fillinpitsandflats(idown[i],jup[j]);
      fillinpitsandflats(idown[i],jdown[j]);
      fillinpitsandflats(iup[i],jdown[j]);}
}

main ()
{    FILE *fp1,*fp2;
     int i,j;
     
     fp1=fopen("inputdem","r");
     fp2=fopen("filleddem","w");
     lattice_size_x=300;
     lattice_size_y=300;
     setupgridneighbors();
     topo=matrix(1,lattice_size_x,1,lattice_size_y);
     for (j=1;j<=lattice_size_y;j++)
      for (i=1;i<=lattice_size_x;i++)
       fscanf(fp1,"%f ",&topo[i][j]);
     for (j=1;j<=lattice_size_y;j++)
      for (i=1;i<=lattice_size_x;i++)
       fillinpitsandflats(i,j);
     for (j=1;j<=lattice_size_y;j++)
      for (i=1;i<=lattice_size_x;i++)
       fprintf(fp2,"%f\n",topo[i][j]);
}    
