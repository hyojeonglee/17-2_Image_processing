// Lab04_Filtering.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <stdio.h>
#include <windows.h>
#define BYTE unsigned char 

int _tmain(int argc, _TCHAR* argv[])
{
	FILE *infile;
	infile=fopen("Mandrill.bmp", "rb");
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

	float M[3][3];
	
	for(int i = 0 ; i < 3 ; i++) {
		for(int j = 0 ; j < 3 ; j++) {
			M[i][j] = 1./9;
			printf("%f", M[i][j]);
		}
	}

	for(int i = 0 ; i < hInfo.biHeight ; i++) {
		for(int j = 0 ; j < hInfo.biWidth ; j++) {
			int val[3];
			for(int m = 0 ; m < 3 ; m++)
				val[m] = 0;
			for(int mi = -1 ; mi < 1 ; mi++) {
				for(int mj = -1 ; mj < 1 ; mj++) {
					if((0<i+mi) && (i+mi<hInfo.biHeight) && (0<j+mj) && (j+mj<hInfo.biWidth)) {
						val[0] = val[0] + lpImg2[i+mi][j+mj][0]*M[mi+1][mj+1];
						val[1] = val[1] + lpImg2[i+mi][j+mj][1]*M[mi+1][mj+1];
						val[2] = val[2] + lpImg2[i+mi][j+mj][2]*M[mi+1][mj+1];
					}
				}
			}
			lpOutImg2[i][j][0] = val[0];
			lpOutImg2[i][j][1] = val[1];
			lpOutImg2[i][j][2] = val[2];
		}
	}
					
	///////////////////////////////////////////////////////////////////

	FILE *outfile = fopen("4_Filtering_1.bmp","wb");
	fwrite(&hf,sizeof(char),sizeof(BITMAPFILEHEADER),outfile);
	fwrite(&hInfo,sizeof(char),sizeof(BITMAPINFOHEADER),outfile);
	fwrite(lpOutImg2,sizeof(char),3*256*256, outfile); 
	fclose(outfile);

	return 0;
}