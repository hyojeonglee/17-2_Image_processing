// Lab04_Filtering.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <stdio.h>
#include <windows.h>
#define BYTE unsigned char 

int _tmain(int argc, _TCHAR* argv[])
{
	FILE *infile;
	infile=fopen("Mandrill_g.bmp", "rb");
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

	float M[2][3] = {{0, 0, 7}, {3, 5, 1}};
	float Eg[256][256];
	int threshold = 80;

	for(int i = 0 ; i < 256 ; i++) {
		for(int j = 0 ; j < 256 ; j++) {
			Eg[i][j] = 0;
		}
	}

	for(int i = 0 ; i < hInfo.biHeight ; i++) {
		for(int j = 0 ; j < hInfo.biWidth ; j++) {
			float Ep = 0;
			for(int mi = 0 ; mi <= 1 ; mi++) {
				for(int mj = -1 ; mj <= 1 ; mj++) {
					if(i-mi > 0 && i-mi < 256 && j+mj < 256 && j+mj > 0)
						Ep += Eg[i-mi][j+mj]*M[mi][1-mj]*1./16;	
				}
			}
			int T = lpImg2[i][j][0] + Ep;
			if (T > threshold) {
				for (int c = 0 ; c < 3 ; c++) {
					lpOutImg2[i][j][c] = 255;
				}
				Eg[i][j] = T - 2*threshold;
			}
			else {
				for (int c = 0 ; c < 3 ; c++) {
					lpOutImg2[i][j][c] = 0;
				}
				Eg[i][j] = T;
			}
		}
	}
					
	///////////////////////////////////////////////////////////////////

	FILE *outfile = fopen("5_halftoning_advanced.bmp","wb");
	fwrite(&hf,sizeof(char),sizeof(BITMAPFILEHEADER),outfile);
	fwrite(&hInfo,sizeof(char),sizeof(BITMAPINFOHEADER),outfile);
	fwrite(lpOutImg2,sizeof(char),3*256*256, outfile); 
	fclose(outfile);

	return 0;
}