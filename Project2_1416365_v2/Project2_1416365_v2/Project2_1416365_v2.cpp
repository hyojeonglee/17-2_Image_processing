#include "stdafx.h"
#include <stdio.h>
#include <windows.h>
#define BYTE unsigned char 

int _tmain(int argc, _TCHAR* argv[])
{
	FILE *infile;
	infile = fopen("scan2.bmp", "rb");
	if (infile == NULL) { printf("There is no file!!!\n"); exit(1); }

	BITMAPFILEHEADER hf;
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, infile);
	if (hf.bfType != 0x4D42) exit(1);

	BITMAPINFOHEADER hInfo;
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, infile); 
	printf("Image Size: (%3dx%3d)\n", hInfo.biWidth, hInfo.biHeight);
	printf("Pallete Type: %dbit Colors\n", hInfo.biBitCount);

	if (hInfo.biBitCount<8) { printf("Bad File format!!"); exit(1); }
	RGBQUAD *pRGB;
	if (hInfo.biClrUsed != 0)
	{
		pRGB = new RGBQUAD[hInfo.biClrUsed];
		fread(pRGB, sizeof(RGBQUAD), hInfo.biClrUsed, infile);
	}

	BYTE lpImg2[256][256][3];
	fread(lpImg2, sizeof(char), hInfo.biSizeImage, infile);
	fclose(infile);
	BYTE lpOutImg2[256][256][3];

	BYTE temp;
	double x, y;
	double int_x1, int_x2, int_y;
	int lx, rx, ty, by;

	for (int i = 0; i<256; i++)
	{
	  for (int j = 0; j<256; j++)
	  {
	    for (int c = 0; c<3; c++)
	    {
	      double w = -0.0063*double(j) + 0.0569*double(i) - 81.6835;
	      x = (-53.9*double(j) + 7.5291*double(i) - 4002.5) / w;
	      y = (-0.7606*double(j) + -48.825*double(i) - 1388.6) / w;

	      ty = int(y);
	      lx = int(x);
	      by = ty + 1;
	      rx = lx + 1;

	      if (ty>-1&&ty<256&&by>-1&&by<256&&lx>-1&&lx<256&&rx>-1&&rx<256)
	      {
	        double inputTopLeft = double(lpImg2[ty][lx][c]);
	        double inputTopRight = double(lpImg2[ty][rx][c]);
	        double inputBotLeft = double(lpImg2[by][lx][c]);
	        double inputBotRight = double(lpImg2[by][rx][c]);
	        int_x1 = (rx - x)*inputTopLeft + (x - lx)*inputTopRight;
	        int_x2 = (rx - x)*inputBotLeft + (x - lx)*inputBotRight;
	        int_y = (by - y)*int_x1 + (y - ty)*int_x2;
	        lpOutImg2[i][j][c] = unsigned char(int_y);
	      }
	    }
	  }
	}


	///////////////////////////////////////////////////////////////////

	FILE *outfile = fopen("hw2_1416365.bmp", "wb");
	fwrite(&hf, sizeof(char), sizeof(BITMAPFILEHEADER), outfile);
	fwrite(&hInfo, sizeof(char), sizeof(BITMAPINFOHEADER), outfile);
	fwrite(lpOutImg2, sizeof(char), 3 * 256 * 256, outfile);
	fclose(outfile);

	return 0;
}
