#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	FILE *fp;
	size_t byte, byteOffset, lengthFile;

	byteOffset = 0;
	lengthFile = 0;

	switch (argc) {
	case 4:
		byteOffset = strtoul(argv[2], 0, 16);
		lengthFile = strtoul(argv[3], 0, 16) + byteOffset;
		break;
	case 3:
		byteOffset = strtoul(argv[2], 0, 16);
		break;
	case 2:
		break;
	default:
		printf("usage: %s file [byte offset] [# of bytes] ['| less -R']\n", argv[0]);
		return 1;
	}

	if ((fp = fopen(argv[1], "r")) == NULL) {
		perror(argv[1]);
		return 2;
	}

	if ((signed long) lengthFile <= 0) {
		fseek(fp, 0, SEEK_END);
		lengthFile = ftell(fp);
	}
	fseek(fp, byteOffset, SEEK_SET);

	for (byte = byteOffset; byte < lengthFile; byte++) {
		unsigned char chara;
		chara = (unsigned char) fgetc(fp);

		printf("%02X", chara);

		/* printable ascii */
		if (chara >= 32 && chara <= 126)
			printf("\033[91m%c\033[0m ", chara);
		/* non-printable ascii */
		else
			fwrite(". ", 2, 1, stdout);

		/* print middle split */
		if (byte - byteOffset == 7) {
			putchar(32);
		/* print byte offset */
		} else if (byte - byteOffset == 15) {
			printf(" %lX B\n", byteOffset);
			byteOffset += 16;
		}
	}

	fclose(fp);
	putchar(10);

	return 0;
}
