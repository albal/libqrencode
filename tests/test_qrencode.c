#include <stdio.h>
#include <string.h>
#include "common.h"
#include "../qrspec.h"
#include "../qrtest.h"

void test_iterate()
{
	int i;
	QRenc_DataStream *stream;
	char num[9] = "01234567";
	unsigned char *data;
	QRRawCode *raw;
	int err = 0;

	testStart("Test getCode (1-L)");
	stream = QRenc_newData();
	QRenc_setVersion(stream, 1);
	QRenc_setErrorCorrectionLevel(stream, QR_EC_LEVEL_L);
	QRenc_appendData(stream, QR_MODE_NUM, 8, (unsigned char *)num);

	raw = QRraw_new(stream);
	data = raw->datacode;
	for(i=0; i<raw->dataLength; i++) {
		if(data[i] != QRraw_getCode(raw)) {
			err++;
		}
	}

	QRenc_freeData(stream);
	QRraw_free(raw);
	testEnd(err);
}

void test_iterate2()
{
	int i;
	QRenc_DataStream *stream;
	char num[9] = "01234567";
	QRRawCode *raw;
	int err = 0;
	unsigned char correct[] = {
	0x10, 0x11, 0xec, 0xec, 0x20, 0xec, 0x11, 0x11,
	0x0c, 0x11, 0xec, 0xec, 0x56, 0xec, 0x11, 0x11,
	0x61, 0x11, 0xec, 0xec, 0x80, 0xec, 0x11, 0x11,
	0xec, 0x11, 0xec, 0xec, 0x11, 0xec, 0x11, 0x11,
	0xec, 0x11, 0xec, 0xec, 0x11, 0xec, 0x11, 0x11,
	0xec, 0x11, 0xec, 0xec, 0x11, 0x11,
	0x5c, 0xde, 0x68, 0x68, 0x4d, 0xb3, 0xdb, 0xdb,
	0xd5, 0x14, 0xe1, 0xe1, 0x5b, 0x2a, 0x1f, 0x1f,
	0x49, 0xc4, 0x78, 0x78, 0xf7, 0xe0, 0x5b, 0x5b,
	0xc3, 0xa7, 0xc1, 0xc1, 0x5d, 0x9a, 0xea, 0xea,
	0x48, 0xad, 0x9d, 0x9d, 0x58, 0xb3, 0x3f, 0x3f,
	0x10, 0xdb, 0xbf, 0xbf, 0xeb, 0xec, 0x05, 0x05,
	0x98, 0x35, 0x83, 0x83, 0xa9, 0x95, 0xa6, 0xa6,
	0xea, 0x7b, 0x8d, 0x8d, 0x04, 0x3c, 0x08, 0x08,
	0x64, 0xce, 0x3e, 0x3e, 0x4d, 0x9b, 0x30, 0x30,
	0x4e, 0x65, 0xd6, 0xd6, 0xe4, 0x53, 0x2c, 0x2c,
	0x46, 0x1d, 0x2e, 0x2e, 0x29, 0x16, 0x27, 0x27
	};

	testStart("Test getCode (5-H)");
	stream = QRenc_newData();
	QRenc_setVersion(stream, 5);
	QRenc_setErrorCorrectionLevel(stream, QR_EC_LEVEL_H);
	QRenc_appendData(stream, QR_MODE_NUM, 8, (unsigned char *)num);

	raw = QRraw_new(stream);
	for(i=0; i<raw->dataLength; i++) {
		if(correct[i] != QRraw_getCode(raw)) {
			err++;
		}
	}

	QRenc_freeData(stream);
	QRraw_free(raw);
	testEnd(err);
}

void print_filler(void)
{
	int width;
	int x, y;
	int version = 5;
	unsigned char *frame;

	width = QRspec_getWidth(version);
	frame = QRenc_fillerTest(version);

	for(y=0; y<width; y++) {
		for(x=0; x<width; x++) {
			printf("%02x ", *frame++);
		}
		printf("\n");
	}
}

void test_filler(void)
{
	int i;
	unsigned char *frame;
	int err = 0;
	int j, w, e;

	testStart("Frame fillter test");
	for(i=1; i<=QRSPEC_VERSION_MAX; i++) {
		frame = QRenc_fillerTest(i);
		if(frame == NULL) {
			printf("Something wrong in version %d\n", i);
			err++;
		} else {
			w = QRspec_getWidth(i);
			e = 0;
			for(j=0; j<w*w; j++) {
				if(frame[j] == 0) e++;
			}
			free(frame);
			if(e) {
				printf("Non-filled bit was found in version %d\n", i);
				err++;
			}
		}

	}
	testEnd(err);
}

void print_mask(void)
{
	int mask;
	int x, y;
	int version = 4;
	int width;
	unsigned char *frame, *masked, *p;

	width = QRspec_getWidth(version);
	frame = (unsigned char *)malloc(width * width);
	memset(frame, 0x20, width * width);
	for(mask=0; mask<8; mask++) {
		masked = QRenc_mask(width, frame, mask);
		p = masked;
		printf("mask %d:\n", mask);
		for(y=0; y<width; y++) {
			for(x=0; x<width; x++) {
				if(*p & 1) {
					printf("#");
				} else {
					printf(" ");
				}
				p++;
			}
			printf("\n");
		}
		printf("\n");
		free(masked);
	}
	free(frame);
}

int main(int argc, char **argv)
{
	test_iterate();
	test_iterate2();
//	print_filler();
	test_filler();
//	print_mask();

	report();

	return 0;
}
