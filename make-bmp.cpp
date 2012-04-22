#include <iostream>
#include <string.h>
#include <stdint.h>
#include <fstream>

struct bmpfile_magic {
  unsigned char magic[2];
};
 
struct bmpfile_header {
  uint32_t filesz;
  uint16_t creator1;
  uint16_t creator2;
  uint32_t bmp_offset;
};

struct bmpfile_info_header {
  uint32_t header_sz;
  int32_t width;
  int32_t height;
  uint16_t nplanes;
  uint16_t bitspp;
  uint32_t compress_type;
  uint32_t bmp_bytesz;
  int32_t hres;
  int32_t vres;
  uint32_t ncolors;
  uint32_t nimpcolors;
};

// see http://en.wikipedia.org/wiki/BMP_file_format

using namespace std;

int main() {
	struct bmpfile_magic magic;
	struct bmpfile_header header;
	struct bmpfile_info_header info;
	const char* data = "Hello, World!";
	int size = strlen(data);
	int width = (size+2)/3;
//	size = width*3;
	int padding = ((((4-size)%4)+4)%4);
	int psize = size+padding;

	cout << "data size:  " << size << endl;
	cout << "padding:    " << padding << endl;
	cout << "padded size:" << psize << endl;

	const int colors = 0;
	const int offset = sizeof(struct bmpfile_info_header)+
			   sizeof(struct bmpfile_header)+
			   sizeof(struct bmpfile_magic)+
			   colors;
	
	magic.magic[0] = 0x42;
	magic.magic[1] = 0x4D;

	header.filesz = offset+psize;
	header.creator1 = 0;
	header.creator2 = 0;
	header.bmp_offset = offset;

	info.header_sz = sizeof(struct bmpfile_info_header);
	info.width = size/3; // width;
	info.height = 1;
	info.nplanes = 1;
	info.bitspp = 24;
	info.compress_type = 0;
	info.bmp_bytesz = psize;
	info.hres = 1;
	info.vres = 1;
	info.ncolors = colors;
	info.nimpcolors = 0;

//	uint32_t color[1<<8];
//	for(int i=0 ; i<colors ; ++i) {
//		color[i] = i<<8;
////	}

	ofstream f("output.bmp");

	f.write((char*)&magic,sizeof(struct bmpfile_magic));
        f.write((char*)&header,sizeof(struct bmpfile_header));
        f.write((char*)&info,sizeof(struct bmpfile_info_header));
//	f.write((char*)&color,colors*4);

	f.write(data,size);
	
	f.close();

	return 0;
}
