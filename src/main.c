/*
 *
 * main.c - Valve Interface Query
 *
 */
#include <pthread.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

void usage();
void add_to_ld_path(const char* entry);

typedef void*(*pfnCreateInterface_t)(const char*,int*);

const char* pextra_dirs[32];

int main(int argc, char** argv)
{
	int b_interface = 0;
	char* pinterface = NULL;
	char* pdll = NULL;

	memset(pextra_dirs, 0, sizeof(char*) * 32);

	/* Parse the arguments */
	for(int i = 1; i < argc; i++)
	{
		char* arg = argv[i];
		
		/* Check for -l */
		if(strncmp(arg, "-l", 2) == 0)
		{
			if(i < argc-1)
			{
				add_to_ld_path(argv[++i]);
			}
			else
				usage();
			continue;
		}

		/* Check for -i */
		if(strncmp(arg, "-i", 2) == 0)
		{
			if(b_interface) usage();

			b_interface = 1;
			continue;
		}

		if(b_interface)
		{
			pinterface = arg;
			b_interface = 0;
			continue;
		}

		if(pdll) usage();
		pdll = arg;
	}
	/* If the shared object is null, die! */
	if(!pdll) usage();
	if(!pinterface) usage();

	/* Print out the LD_LIBRARY_PATH */
	const char* ldpath = getenv("LD_LIBRARY_PATH");
	printf("LD_LIBRARY_PATH=%s\n", ldpath ? ldpath : "");

	/* Load the shared object */
	void* module = dlopen(pdll, RTLD_LAZY);

	if(!module)
	{
		printf("Failed to load module.\nError message: %s\n", dlerror());
		exit(1);
	}

	pfnCreateInterface_t pfnCreateInterface = dlsym(module, "CreateInterface");

	if(!pfnCreateInterface)
	{
		printf("Failed to lookup pfnCreateInterface.\nError message: %s\n", dlerror());
		exit(1);
	}

	int ret = 0;
	void* res = pfnCreateInterface(pinterface, &ret);

	printf("CreateInterface returned: pointer 0x%LX, status=%s\n", (unsigned long long)res, ret ? "IFACE_FAILED" : "IFACE_OK");

	return 0;
}

void usage()
{
	printf("USAGE: valve-interface-query -i interface mybinary.so");
	printf("\n\nOptions:\n");
	printf("\t-i <interface>    - Specifies the interface\n");
	printf("\t-l <path>         - Adds a path to your LD_LIBRARY_PATH\n");
	exit(0);
}

void add_to_ld_path(const char* entry)
{
	char* tmpstring;
	size_t length;
	const char* pLDPath = getenv("LD_LIBRARY_PATH");

	if(pLDPath)
	{
		length = strlen(pLDPath) + strlen(entry) + 5;
		tmpstring = malloc(length);

		/* Print in the new LD path */
		snprintf(tmpstring, length, "%s:%s", entry, pLDPath);
		setenv("LD_LIBRARY_PATH", tmpstring, 1);
		free(tmpstring);
	}
	else
		setenv("LD_LIBRARY_PATH", entry, 1);

}
