#include "util/file.hpp"
#include "util/ifcrash.hpp"
#include <stdint.h>
#include <stdio.h>


bool util::loadFile(
	const char*   path, /* filepath 													*/ 
	unsigned int* size, /* the size of the out array that was pre-allocated.            */
	char*		  out   /* Where to place the file contents (Buffer of Min-Size 'size') */
) {
	uint64_t fsize;
	FILE* to_open = nullptr;

	to_open = fopen(path, "rb");
	ifcrashfmt(!to_open, "Couldn't get handle to file at Path [%s]\n", path);
	fseek(to_open, 0, SEEK_END);
	fsize = ftell(to_open);
	fseek(to_open, 0, SEEK_SET);

	if(*size < fsize || out == nullptr) {
		/* A buffer wasn't allocated OR the size of the buffer is too small. */
		*size = fsize;
		fclose(to_open);
		return false;
	}


	fsize = fread(out, sizeof(unsigned char), *size, to_open);
	ifcrashdo(fsize != *size, {
		fclose(to_open); 
		printf("Something went wrong - file size is %llu bytes, read only %llu bytes\n", __scast(uint64_t, *size), fsize);
	});


	fsize = fclose(to_open);
	ifcrashfmt(fsize, "Couldn't close file handle. ERROR CODE: %llu\n", fsize);
	return true;
}