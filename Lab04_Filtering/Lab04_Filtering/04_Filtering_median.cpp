// Lab04_Filtering.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <stdio.h>
#include <windows.h>
#define BYTE unsigned char 

int _tmain(int argc, _TCHAR* argv[])
{
	FILE *infile;
	infile=fopen("pepper_noise.bmp", "rb");
	if(infile==NULL) { printf("There is no file!!!\n"); exit(1); }

	BITMAPFILEHEADER hf;
	fread(&hf,sizeof(BITMAPFILEHEADER),1,infile);
	if(hf.bfType!=0x4D42) exit(1); 

	BITMAPINFOHEADER hInfo;
	fread(&hInfo,sizeof(BITMAPINFOHEADER),1,infile);
	printf("Image Size: (%3dx%3d)\n",hInfo.biWidth,hInfo.biHeight);
	printf("Pallete Type: %dbit Colors\n",hInfo.biBitCount);

	if(hInfo.biBitCount<8 ) { printf("Bad File format!!"); exit(1); }
	RGBQUAD *pRGB; 
	if(hInfo.biClrUsed!=0)
	{
		pRGB= new RGBQUAD [hInfo.biClrUsed]; 
		fread(pRGB,sizeof(RGBQUAD),hInfo.biClrUsed,infile); 
	}

	
	BYTE lpImg2[256][256][3];
	fread(lpImg2,sizeof(char),hInfo.biSizeImage,infile); 
	fclose(infile);

	BYTE lpOutImg2[256][256][3];

	int A[9][3];

	for(int i = 0 ; i < hInfo.biHeight ; i++) {
		for(int j = 0 ; j < hInfo.biWidth ; j++) {
			int count = 0;
			for(int mi = i-1 ; mi <= i+1 ; mi++) {
				for(int mj = j-1 ; mj <= j+1 ; mj++) {
					A[count][0] = lpImg2[mi][mj][0];
					A[count][1] = lpImg2[mi][mj][1];
					A[count][2] = lpImg2[mi][mj][2];
					count++;
				}
			}
			for(int x = 0 ; x < 9 ; x++) {
				for(int y = x+1 ; y < 9 ; y++) {
					if(A[x][0] > A[y][0]) {
						int temp = A[x][0];
						A[x][0] = A[y][0];
						A[y][0] = temp;
					}
					if(A[x][1] > A[y][1]) {
						int temp = A[x][1];
						A[x][1] = A[y][1];
						A[y][1] = temp;
					}
					if(A[x][2] > A[y][2]) {
						int temp = A[x][2];
						A[x][2] = A[y][2];
						A[y][2] = temp;
					}
				}
			}
			lpOutImg2[i][j][0] = A[4][0];
			lpOutImg2[i][j][1] = A[4][1];
			lpOutImg2[i][j][2] = A[4][2];
		}
	}
					
	///////////////////////////////////////////////////////////////////

	FILE *outfile = fopen("4_Filtering_3.bmp","wb");
	fwrite(&hf,sizeof(char),sizeof(BITMAPFILEHEADER),outfile);
	fwrite(&hInfo,sizeof(char),sizeof(BITMAPINFOHEADER),outfile);
	fwrite(lpOutImg2,sizeof(char),3*256*256, outfile); 
	fclose(outfile);

	return 0;
}