#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<time.h>
#define error(m, c) do{fprintf(stderr, "%s\n", m);exit(c);}while(0)
void main(int argc, char** argv){
	if(argc != 3)
		error("Fatal error. Usage: puzzle row_no col_no", 1);
	long n = atoi(argv[1]);
	long m = atoi(argv[2]);
	long k = n * m;
	srand(time(NULL));
	long l = 0;
	while(l++ < k){
		putchar('a'+rand()%26);
		if(l%m == 0 && l != k)
			putchar('\n');
	}
}