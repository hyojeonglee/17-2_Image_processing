#include "stdafx.h"
#include <stdio.h>
#include <windows.h>
#include <math.h>
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

	float expand = 2.;
	int lpImgLength = 256 / expand;

	// backward Bipolar Interpolation
	for(int i = 0 ; i < 256 ; i++) {
		for(int j = 0 ; j < 256 ; j++) {
			float from_i = i / expand;
			float from_j = j / expand;

			int test_i = i / expand;
			int test_j = j / expand;

			/*if (from_i == float(test_i) && from_j == float(test_j)) {
				for(int c = 0 ; c < 3 ; c++)
					lpOutImg2[i][j][c] = lpImg2[int(from_i)][int(from_j)][c];
				
			}
			else {*/
				/*if (ceil(from_i) >= lpImgLength || floor(from_i) < 0 || ceil(from_j) >= lpImgLength || floor(from_j) < 0) {
					for(int c = 0 ; c < 3 ; c++) {
						lpOutImg2[i][j][c] = 0;
					}
				}*/
				//else {
					for(int c = 0 ; c < 3 ; c++) {
						int high_i = ceil(from_i);
						int low_i = floor(from_i);

						float a = from_i - low_i;
						float b = high_i - from_i;

						float y1 = lpImg2[high_i][j][c]*a + lpImg2[low_i][j][c]*b;
						float y2 = lpImg2[high_i+1][j][c]*a + lpImg2[low_i+1][j][c]*b;

						lpOutImg2[i][j][c] = y1*a + y2*b;
					}
				//}
			//}
		}
	}
			
	///////////////////////////////////////////////////////////////////

	FILE *outfile = fopen("BipolarInterpolation.bmp","wb");
	fwrite(&hf,sizeof(char),sizeof(BITMAPFILEHEADER),outfile);
	fwrite(&hInfo,sizeof(char),sizeof(BITMAPINFOHEADER),outfile);
	fwrite(lpOutImg2,sizeof(char),3*256*256, outfile); 
	fclose(outfile);

	return 0;
}