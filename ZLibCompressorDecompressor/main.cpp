#include <errno.h>
#include <iostream>
#include <stdbool.h>
#include <zlib/zlib.h>

const int standardBufferSize = 128;

unsigned long fileSize(char* fileName)
{
	FILE* pFile;
	errno_t err;

	if (err = fopen_s(&pFile, fileName, "rb")!= 0)
	{
		std::cout << "Could not determine file size!" << std::endl;
		return 0;
	}
	else
	{
		fseek(pFile, 0, SEEK_END);
		unsigned long size = ftell(pFile);
		fclose(pFile);
		return size;
	}
}

bool CompressFile(char* inFilename, char* outFilename)
{
	FILE* infile;
	gzFile outfile;
	errno_t err;

	if ((err = fopen_s(&infile, inFilename, "rb") != 0))
	{
		std::cout << "Could not open in file for compression!" << std::endl;
		return false;
	}

	outfile = gzopen(outFilename, "wb");

	if (!outfile)
	{
		std::cout << "Could not open out file for compression!" << std::endl;
		return false;
	}
	else
	{
		char inbuffer[standardBufferSize];
		int numRead = 0;
		unsigned long totalRead = 0, totalWrote = 0;

		while ((numRead = static_cast<int>(fread(inbuffer, 1, sizeof(inbuffer), infile))) > 0)
		{
			totalRead += numRead;
			gzwrite(outfile, inbuffer, numRead);
		}

		fclose(infile);
		gzclose(outfile);

		return true;
	}
}

bool DecompressFile(char* inFileName, char* outFileName)
{
	gzFile infile;
	FILE* outfile;
	errno_t err;

	infile = gzopen(inFileName, "rb");

	if (!infile)
	{
		std::cout << "Could not read in file for decompression!" << std::endl;
		return false;
	}

	if ((err = fopen_s(&outfile, outFileName, "wb") != 0))
	{
		std::cout << "Could open out file for decompressing image!" << std::endl;
		return false;
	}
	else
	{
		char buffer[standardBufferSize];
		int numRead = 0;

		while ((numRead = gzread(infile, buffer, sizeof(buffer))) > 0)
		{
			fwrite(buffer, 1, numRead, outfile);
		}

		gzclose(infile);
		fclose(outfile);

		return true;
	}
}

int main()
{
	char inputPath[] = "Input/image.png";
	char compressedPath[] = "Storage/image.dat";
	char decompressedPath[] = "Output/imageOut.png";

	if (CompressFile(inputPath, compressedPath) == false)
	{
		std::cout << "Could not compress image!" << std::endl;
		return -1;
	}
		
	if (DecompressFile(compressedPath, decompressedPath) == false)
	{
		std::cout << "Could not decompress image!" << std::endl;
		return -1;
	}

	std::cout << "Compression successful!" << std::endl;

	return 0;
}