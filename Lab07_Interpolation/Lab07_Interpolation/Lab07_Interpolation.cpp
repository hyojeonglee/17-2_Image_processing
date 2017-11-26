#include "stdafx.h"
#include <stdio.h>
#include <windows.h>
#define BYTE unsigned char 

int _tmain(int argc, _TCHAR* argv[])
{
	FILE *infile;
	infile = fopen("Mandrill.bmp", "rb");
	if (infile == NULL) { printf("There is no file!!!\n"); exit(1); }

	BITMAPFILEHEADER hf;
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, infile); // ������带 ���� 
	if (hf.bfType != 0x4D42) exit(1);

	BITMAPINFOHEADER hInfo;
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, infile); // ������带 ���� 
	printf("Image Size: (%3dx%3d)\n", hInfo.biWidth, hInfo.biHeight);
	printf("Pallete Type: %dbit Colors\n", hInfo.biBitCount);


	// 256Į�� ������ ���� ������� ����
	if (hInfo.biBitCount<8) { printf("Bad File format!!"); exit(1); }
	RGBQUAD *pRGB;
	if (hInfo.biClrUsed != 0) // �ȷ�Ʈ�� �ִ� ��� 
	{
		pRGB = new RGBQUAD[hInfo.biClrUsed]; // �ȷ�Ʈ�� ũ�⸸ŭ �޸𸮸� �Ҵ��� 
		fread(pRGB, sizeof(RGBQUAD), hInfo.biClrUsed, infile); // �ȷ�Ʈ�� ���Ͽ��� ���� 
	}

	// ������Ÿ�� ������ �޸� �Ҵ� 

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
				x = double(j) / 2;
				y = double(i) / 1;

				ty = int(y);
				lx = int(x);
				by = ty + 1;
				rx = lx + 1;

				if (ty>-1 && ty<256 && by>-1 && by<256 && lx>-1 && lx<256 && rx>-1 && rx<256)
				{
					int_x1 = (rx - x)*double(lpImg2[ty][lx][c]) + (x - lx)*double(lpImg2[ty][rx][c]);
					int_x2 = (rx - x)*double(lpImg2[by][lx][c]) + (x - lx)*double(lpImg2[by][rx][c]);
					int_y = (by - y)*int_x1 + (y - ty)*int_x2;
					lpOutImg2[i][j][c] = unsigned char(int_y);
				}
			}

		}
	}


	///////////////////////////////////////////////////////////////////

	// ���� ��� (24��Ʈ�� Ʈ��Į��� ���) 
	FILE *outfile = fopen("OutImg24.bmp", "wb");
	fwrite(&hf, sizeof(char), sizeof(BITMAPFILEHEADER), outfile);
	fwrite(&hInfo, sizeof(char), sizeof(BITMAPINFOHEADER), outfile);
	fwrite(lpOutImg2, sizeof(char), 3 * 256 * 256, outfile);
	fclose(outfile);

	return 0;
}
