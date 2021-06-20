/*
 *
 * main.c - Valve Interface Query
 *
 */
#ifdef _UNIX 
#include <pthread.h>
#include <dlfcn.h>
#include <unistd.h>
#else
#include <windows.h>
#endif 

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

void usage();

/* Platform abstracted funcs */
void* Plat_LoadLibrary(const char* lib);
void* Plat_FindSymbol(void* lib, const char* sym);
const char* Plat_LastError();

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
		
		/* Check for -c */
		if(strncmp(arg, "-c", 2) == 0)
		{
			if(i < argc-1)
			{
				chdir(argv[++i]);
				printf("Changed dir to %s\n", argv[i]);
			}
			else
				usage();
			continue;
		}

		/* Check for help */
		if(strncmp(arg, "-h", 2) == 0 || strncmp(arg, "--help", 6) == 0)
		{
			usage();
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

#ifndef _WIN32 
	/* Print out the LD_LIBRARY_PATH */
	const char* ldpath = getenv("LD_LIBRARY_PATH");
	printf("LD_LIBRARY_PATH=%s\n", ldpath ? ldpath : "");
#endif 

	/* Load the shared object */
	void* module = Plat_LoadLibrary(pdll);

	if(!module)
	{
		printf("Failed to load module.\nError message: %s\n", Plat_LastError());
		exit(1);
	}

	pfnCreateInterface_t pfnCreateInterface = Plat_FindSymbol(module, "CreateInterface");

	if(!pfnCreateInterface)
	{
		printf("Failed to lookup pfnCreateInterface.\nError message: %s\n", Plat_LastError());
		exit(1);
	}

	int ret = 0;
	void* res = pfnCreateInterface(pinterface, &ret);

	printf("CreateInterface returned: pointer 0x%LX, status=%s\n", (uintptr_t)res, ret ? "IFACE_FAILED" : "IFACE_OK");

	return ret;
}

void usage()
{
	printf("USAGE: valve-interface-query -i interface mybinary.so");
	printf("\n\nOptions:\n");
	printf("\t-i <interface>    - Specifies the interface\n");
	exit(0);
}

void* Plat_LoadLibrary(const char* lib)
{
#ifdef _WIN32 
	return LoadLibraryA(lib);
#else
	char realPath[1024];
	*realPath = 0;
	// Make absolute if not already
	if(*lib != '/') 
	{
		getcwd(realPath, sizeof(realPath));
		strncat(realPath, "/", sizeof(realPath)-1);
		strncat(realPath, lib, sizeof(realPath)-1);
	}
	else
	{
		strncpy(realPath, lib, sizeof(realPath));
	}
	return dlopen(realPath, RTLD_LAZY);
#endif 
}

void* Plat_FindSymbol(void* lib, const char* sym)
{
#ifdef _WIN32 
	return GetProcAddress(lib, sym);
#else 
	return dlsym(lib, sym);
#endif
}

const char* Plat_LastError()
{
#ifdef _WIN32
	static char buf[1024];
	snprintf(buf, 1024, "Error code %u", GetLastError());
	return buf;
#else
	return dlerror();
#endif 
}
