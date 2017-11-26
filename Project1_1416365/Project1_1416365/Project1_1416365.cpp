#include "stdafx.h"
#include <stdio.h>
#include <windows.h>
#include <math.h>
#define BYTE unsigned char 

int _tmain(int argc, _TCHAR* argv[])
{
	FILE *infile;
	infile = fopen("test.bmp", "rb");
	if (infile == NULL) { printf("There is no file!!!\n"); exit(1); }

	BITMAPFILEHEADER hf;
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, infile);
	if (hf.bfType != 0x4D42) exit(1);

	BITMAPINFOHEADER hInfo;
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, infile);
	printf("Image Size: (%3dx%3d)\n", hInfo.biWidth, hInfo.biHeight);
	printf("Pallete Type: %dbit Colors\n", hInfo.biBitCount);


	// Ignore under 256-color
	if (hInfo.biBitCount<8) { printf("Bad File format!!"); exit(1); }
	RGBQUAD *pRGB;
	if (hInfo.biClrUsed != 0)
	{
		pRGB = new RGBQUAD[hInfo.biClrUsed];
		fread(pRGB, sizeof(RGBQUAD), hInfo.biClrUsed, infile);
	}

	// Memory for Image Data
	BYTE lpImg2[256][256][3];
	fread(lpImg2, sizeof(char), hInfo.biSizeImage, infile);
	fclose(infile);
	BYTE lpOutImg2[256][256][3];

	///////////////////////////////////////////////////////////////////

	// (1) Calculate hist and cum_hist
	int hist[256], chist[256];
	for (int i = 0; i<256; i++)
	{
		hist[i] = 0;
		chist[i] = 0;
	}
	for (int i = 0; i<256; i++)
		for (int j = 0; j<256; j++)
			hist[lpImg2[i][j][0]]++;

	int sum = 0;
	for (int i = 0; i<256; i++) {
		sum = sum + hist[i];
		chist[i] = sum;
	}

	// (2) Auto Binarization
	float prob[256];
	for (int i = 0; i<256; i++)
		prob[i] = float(hist[i]) / (256 * 256);

	float q1, q2, m1, m2, d1, d2, dw;
	float var = 10000000000000.0f;
	int T;
	for (int t = 0; t<256; t++) {
		q1 = 0.0f;
		q2 = 0.0f;
		m1 = 0.0f;
		m2 = 0.0f;
		d1 = 0.0f;
		d2 = 0.0f;
		for (int i = 0; i<t; i++)
			q1 = q1 + prob[i];
		for (int i = t; i<256; i++)
			q2 = q2 + prob[i];
		if (q1 == 0 || q2 == 0) continue;

		for (int i = 0; i<t; i++)
			m1 = m1 + i*prob[i] / q1;

		for (int i = t; i<256; i++)
			m2 = m2 + i*prob[i] / q2;

		for (int i = 0; i<t; i++)
			d1 = d1 + (i - m1)*(i - m1)*prob[i] / q1;

		for (int i = t; i<256; i++)
			d2 = d2 + (i - m2)*(i - m2)*prob[i] / q2;

		dw = q1*d1 + q2*d2;

		if (dw<var) {
			T = t;
			var = dw;
		}
	}
	printf("\nThreshold : %d\n", T);

	// (3) Apply HDR and Create Output
	int k = 0;
	for (int i = 0; i < hInfo.biHeight; i++) {
		for (int j = 0; j < hInfo.biWidth; j++) {
			k = lpImg2[i][j][0];
			for (int c = 0; c < 3; c++) {
				if (k > T)
					lpOutImg2[i][j][c] = 255;
				else
					lpOutImg2[i][j][c] = int(255 * (chist[k] / double(chist[T])));
			}
		}
	}


	///////////////////////////////////////////////////////////////////

	// Print Image (24bit True color) 
	FILE *outfile = fopen("test1.bmp", "wb");
	fwrite(&hf, sizeof(char), sizeof(BITMAPFILEHEADER), outfile);
	fwrite(&hInfo, sizeof(char), sizeof(BITMAPINFOHEADER), outfile);
	fwrite(lpOutImg2, sizeof(char), 3 * 256 * 256, outfile);
	fclose(outfile);

	return 0;
}
