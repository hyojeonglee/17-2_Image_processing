// ����ó��.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
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
	fread(&hf,sizeof(BITMAPFILEHEADER),1,infile); // ������带 ���� 
	if(hf.bfType!=0x4D42) exit(1); 

	BITMAPINFOHEADER hInfo;
	fread(&hInfo,sizeof(BITMAPINFOHEADER),1,infile); // ������带 ���� 
	printf("Image Size: (%3dx%3d)\n",hInfo.biWidth,hInfo.biHeight);
	printf("Pallete Type: %dbit Colors\n",hInfo.biBitCount);


	// 256Į�� ������ ���� ������� ����
	if(hInfo.biBitCount<8 ) { printf("Bad File format!!"); exit(1); }
	RGBQUAD *pRGB; 
	if(hInfo.biClrUsed!=0) // �ȷ�Ʈ�� �ִ� ��� 
	{
		pRGB= new RGBQUAD [hInfo.biClrUsed]; // �ȷ�Ʈ�� ũ�⸸ŭ �޸𸮸� �Ҵ��� 
		fread(pRGB,sizeof(RGBQUAD),hInfo.biClrUsed,infile); // �ȷ�Ʈ�� ���Ͽ��� ���� 
	}

	// ������Ÿ�� ������ �޸� �Ҵ� 

	BYTE lpImg2[256][256][3];
	fread(lpImg2,sizeof(char),hInfo.biSizeImage,infile); 
	fclose(infile);
	BYTE lpOutImg2[256][256][3];


	// �ڵ� ���� �κ�

	for(int i=0; i<hInfo.biHeight; i++)
		for(int j=0; j<hInfo.biWidth; j++) 
			for (int c=0; c<3; c++)
				lpOutImg2[i][j][c] = lpImg2[i][j][c];


	///////////////////////////////////////////////////////////////////

	// ���� ��� (24��Ʈ�� Ʈ��Į��� ���) 
	FILE *outfile = fopen("OutImg24.bmp","wb");
	fwrite(&hf,sizeof(char),sizeof(BITMAPFILEHEADER),outfile);
	fwrite(&hInfo,sizeof(char),sizeof(BITMAPINFOHEADER),outfile);
	fwrite(lpOutImg2,sizeof(char),3*256*256, outfile); 
	fclose(outfile);

	return 0;
}

