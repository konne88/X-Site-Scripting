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
	int padding = ((((4-(width*3))%4)+4)%4);
	int psize = width*3+padding;

	cout << "data size:  " << size << endl;
	cout << "width:      " << width << endl;
	cout << "image size: " << width*3 << endl;
	cout << "padding:    " << padding << endl;
	cout << "padded size:" << psize << endl;

	const int offset = sizeof(struct bmpfile_info_header)+
			   sizeof(struct bmpfile_header)+
			   sizeof(struct bmpfile_magic);
	
	magic.magic[0] = 0x42;
	magic.magic[1] = 0x4D;

	header.filesz = offset+psize;
	header.creator1 = 0;
	header.creator2 = 0;
	header.bmp_offset = offset;

	info.header_sz = sizeof(struct bmpfile_info_header);
	info.width = width;
	info.height = 1;
	info.nplanes = 1;
	info.bitspp = 24;
	info.compress_type = 0;
	info.bmp_bytesz = psize;
	info.hres = 1;
	info.vres = 1;
	info.ncolors = 0;
	info.nimpcolors = 0;

	ofstream f("output.bmp");

	f.write((char*)&magic,sizeof(struct bmpfile_magic));
        f.write((char*)&header,sizeof(struct bmpfile_header));
        f.write((char*)&info,sizeof(struct bmpfile_info_header));

	f.write(data,size);

	for(int i=size ; i<psize ; ++i) {
		f.put(0);
	}

	f.close();

	return 0;
}
