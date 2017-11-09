
#include "stdafx.h"
#include <stdio.h>
#include <windows.h>
#define BYTE unsigned char 

int _tmain(int argc, _TCHAR* argv[])
{
	FILE *infile;
	infile=fopen("coin.bmp", "rb");
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

	BYTE lpOutImg3[256][256][3];

	int A[81];

	for(int i = 0 ; i < hInfo.biHeight ; i++) {
		for(int j = 0 ; j < hInfo.biWidth ; j++) {
			int count = 0;
			for(int mi = -4 ; mi <= 4 ; mi++) {
				for(int mj = -4 ; mj <= 4 ; mj++) {
					if(i+mi > -1 && i+mi < 256 && j+mj < 256 && j+mj > -1) {
						A[count] = lpImg2[i+mi][j+mj][0];
						count++;
					}
				}
			}
			for(int x = 0 ; x < count ; x++) {
				for(int y = x+1 ; y < count ; y++) {
					if(A[x] > A[y]) {
						int temp = A[x];
						A[x] = A[y];
						A[y] = temp;
					}
				}
			}
		
			lpOutImg2[i][j][0] = A[count-1];
			lpOutImg2[i][j][1] = A[count-1];
			lpOutImg2[i][j][2] = A[count-1];
		}
	}

	int B[81];

	for(int i = 0 ; i < hInfo.biHeight ; i++) {
		for(int j = 0 ; j < hInfo.biWidth ; j++) {
			int count = 0;
			for(int mi = -4 ; mi <= 4 ; mi++) {
				for(int mj = -4 ; mj <= 4 ; mj++) {
					if(i+mi > -1 && i+mi < 256 && j+mj < 256 && j+mj > -1) {
						B[count] = lpOutImg2[i+mi][j+mj][0];
						count++;
					}
				}
			}
			for(int x = 0 ; x < count ; x++) {
				for(int y = x+1 ; y < count ; y++) {
					if(B[x] > B[y]) {
						int temp = B[x];
						B[x] = B[y];
						B[y] = temp;
					}
				}
			}
		
			lpOutImg3[i][j][0] = B[0];
			lpOutImg3[i][j][1] = B[0];
			lpOutImg3[i][j][2] = B[0];
		}
	}
					
	///////////////////////////////////////////////////////////////////

	FILE *outfile = fopen("Closing.bmp","wb");
	fwrite(&hf,sizeof(char),sizeof(BITMAPFILEHEADER),outfile);
	fwrite(&hInfo,sizeof(char),sizeof(BITMAPINFOHEADER),outfile);
	fwrite(lpOutImg3,sizeof(char),3*256*256, outfile); 
	fclose(outfile);

	return 0;
}