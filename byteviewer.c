#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	FILE *fp;
	long byteOffset, lengthFile, pos;

	byteOffset = 0;
	lengthFile = 0;

	switch (argc) {
	case 4:
		byteOffset = strtol(argv[2], 0, 16);
		lengthFile = strtol(argv[3], 0, 16) + byteOffset;
		break;
	case 3:
		byteOffset = strtol(argv[2], 0, 16);
		break;
	case 2:
		break;
	default:
		printf("usage: %s file [byte offset] [# of bytes] ['| less -R']\n", argv[0]);
		return 1;
	}

	if (byteOffset < 0)
		byteOffset = 0;

	if ((fp = fopen(argv[1], "r")) == NULL) {
		perror(argv[1]);
		return 2;
	}

	if (lengthFile <= 0) {
		fseek(fp, 0, SEEK_END);
		if (ftell(fp) <= 0) {
			printf("%s: empty\n", argv[1]);
			return 3;
		}
		lengthFile = ftell(fp);
	}
	fseek(fp, byteOffset, SEEK_SET);

	for (pos = byteOffset; pos < lengthFile; pos++) {
		unsigned char byte;
		byte = (unsigned char) fgetc(fp);

		printf("%02X", byte);

		/* printable ascii */
		if (byte >= 32 && byte <= 126)
			printf("\033[91m%c\033[0m ", byte);
		/* non-printable ascii */
		else
			fwrite(". ", 2, 1, stdout);

		/* print middle split */
		if ((pos - byteOffset) == 7) {
			putchar(32);
		/* print byte offset */
		} else if ((pos - byteOffset) == 15) {
			printf(" %lX B\n", byteOffset);
			byteOffset += 16;
		}
	}

	fclose(fp);
	putchar(10);

	return 0;
}
