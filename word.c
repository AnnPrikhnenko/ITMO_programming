#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

void Text(FILE* fin) {
	char c;
	printf("\n");
	while ((c = fgetc(fin)) != EOF) {
		printf("%c", c);
	}
	
	rewind(fin);
}

int LinesCount(FILE* fin) {
	int lines = 1;
	char c;
	while ((c = fgetc(fin)) != EOF) {
		if (c == '\n') {
			lines++;
		}
	}
	
	rewind(fin);
	return lines;
}

int WordsCount(FILE* fin) {
	int words = 1;
	char c;
	char prev = 'a';
	while ((c = fgetc(fin)) != EOF) {
		if (c == ' ') {
			if (prev != ' ' && prev != '\n' && prev != '\t') {
				words++;
			}
		} else if (c == '\n') {
			if (prev != ' ' && prev != '\n' && prev != '\t') {
				words++;
			}
		} else if (c == '\t') {
			if (prev != ' ' && prev != '\n' && prev != '\t') {
				words++;
			}
		}
		prev = c;
	}
	if (prev != ' ' || prev != '\n' || prev != '\t') {
		words--;
	}
	
	rewind(fin);
	return words;
}

int BytesCount(FILE* fin) {
	int bytes = 0;
	char c; 
	while ((c = fgetc(fin)) != EOF) {
		if (c == '\n') {
			bytes++;
		}
		bytes++;
	}
	
	rewind(fin);
	return bytes;
}

int main(int argc, char** argv) {
	FILE *fin;
	if (!(fin = fopen(argv[2], "r"))) {
		printf("Error file!");
	}

	if (!(strcmp(argv[1], "--lines")) || !(strcmp(argv[1], "-l"))) {
		printf("\n%i - lines", LinesCount(fin));
	} else if (!(strcmp(argv[1], "--words")) || !(strcmp(argv[1], "-w"))) {
		printf("\n%i - words", WordsCount(fin));
	} else if (!(strcmp(argv[1], "--bytes")) || !(strcmp(argv[1], "-c"))) {
		printf("\n%i - bytes", BytesCount(fin));
	} else if (!(strcmp(argv[1], "--text")) || !(strcmp(argv[1], "-t"))) {
		Text(fin);
	}
	
	fclose(fin);
	return 0;
}
