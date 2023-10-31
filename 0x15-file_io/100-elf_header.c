#include <elf.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void check_elf(unsigned char *new_ident);
void print_magic(unsigned char *new_ident);
void print_class(unsigned char *new_ident);
void print_data(unsigned char *new_ident);
void print_version(unsigned char *new_ident);
void print_abi(unsigned char *new_ident);
void print_osabi(unsigned char *new_ident);
void print_type(unsigned int new_type, unsigned char *new_ident);
void print_entry(unsigned long int new_entry, unsigned char *new_ident);
void close_elf(int new_elf);

/**
 * check_elf - Checks if a file is an ELF file.
 * @new_ident: A pointer to an array containing the ELF magic numbers.
 *
 * Description: If the file is not an ELF file - exit code 98.
 */
void check_elf(unsigned char *new_ident)
{
	int idx;

	for (idx = 0; idx < 4; idx++)
	{
		if (new_ident[idx] != 127 &&
		    new_ident[idx] != 'E' &&
		    new_ident[idx] != 'L' &&
		    new_ident[idx] != 'F')
		{
			dprintf(STDERR_FILENO, "Error: Not an ELF file\n");
			exit(98);
		}
	}
}

/**
 * print_magic - Prints the magic numbers of an ELF header.
 * @new_ident: A pointer to an array containing the ELF magic numbers.
 *
 * Description: Magic numbers are separated by spaces.
 */
void print_magic(unsigned char *new_ident)
{
	int idx;

	printf("  Magic:   ");

	for (idx = 0; idx < EI_NIDENT; idx++)
	{
		printf("%02x", new_ident[idx]);

		if (idx == EI_NIDENT - 1)
			printf("\n");
		else
			printf(" ");
	}
}

/**
 * print_class - Prints the class of an ELF header.
 * @new_ident: A pointer to an array containing the ELF class.
 */
void print_class(unsigned char *new_ident)
{
	printf("  Class:                             ");

	switch (new_ident[EI_CLASS])
	{
	case ELFCLASSNONE:
		printf("none\n");
		break;
	case ELFCLASS32:
		printf("ELF32\n");
		break;
	case ELFCLASS64:
		printf("ELF64\n");
		break;
	default:
		printf("<unknown: %x>\n", new_ident[EI_CLASS]);
	}
}

/**
 * print_data - Prints the data of an ELF header.
 * @new_ident: A pointer to an array containing the ELF class.
 */
void print_data(unsigned char *new_ident)
{
	printf("  Data:                              ");

	switch (new_ident[EI_DATA])
	{
	case ELFDATANONE:
		printf("none\n");
		break;
	case ELFDATA2LSB:
		printf("2's complement, little endian\n");
		break;
	case ELFDATA2MSB:
		printf("2's complement, big endian\n");
		break;
	default:
		printf("<unknown: %x>\n", new_ident[EI_CLASS]);
	}
}

/**
 * print_version - Prints the version of an ELF header.
 * @new_ident: A pointer to an array containing the ELF version.
 */
void print_version(unsigned char *new_ident)
{
	printf("  Version:                           %d",
	       new_ident[EI_VERSION]);

	switch (new_ident[EI_VERSION])
	{
	case EV_CURRENT:
		printf(" (current)\n");
		break;
	default:
		printf("\n");
		break;
	}
}

/**
 * print_osabi - Prints the OS/ABI of an ELF header.
 * @new_ident: A pointer to an array containing the ELF version.
 */
void print_osabi(unsigned char *new_ident)
{
	printf("  OS/ABI:                            ");

	switch (new_ident[EI_OSABI])
	{
	case ELFOSABI_NONE:
		printf("UNIX - System V\n");
		break;
	case ELFOSABI_HPUX:
		printf("UNIX - HP-UX\n");
		break;
	case ELFOSABI_NETBSD:
		printf("UNIX - NetBSD\n");
		break;
	case ELFOSABI_LINUX:
		printf("UNIX - Linux\n");
		break;
	case ELFOSABI_SOLARIS:
		printf("UNIX - Solaris\n");
		break;
	case ELFOSABI_IRIX:
		printf("UNIX - IRIX\n");
		break;
	case ELFOSABI_FREEBSD:
		printf("UNIX - FreeBSD\n");
		break;
	case ELFOSABI_TRU64:
		printf("UNIX - TRU64\n");
		break;
	case ELFOSABI_ARM:
		printf("ARM\n");
		break;
	case ELFOSABI_STANDALONE:
		printf("Standalone App\n");
		break;
	default:
		printf("<unknown: %x>\n", new_ident[EI_OSABI]);
	}
}

/**
 * print_abi - Prints the ABI version of an ELF header.
 * @new_ident: A pointer to an array containing the ELF ABI version.
 */
void print_abi(unsigned char *new_ident)
{
	printf("  ABI Version:                       %d\n",
	       new_ident[EI_ABIVERSION]);
}

/**
 * print_type - Prints the type of an ELF header.
 * @new_type: The ELF type.
 * @new_ident: A pointer to an array containing the ELF class.
 */
void print_type(unsigned int new_type, unsigned char *new_ident)
{
	if (new_ident[EI_DATA] == ELFDATA2MSB)
		new_type >>= 8;

	printf("  Type:                              ");

	switch (new_type)
	{
	case ET_NONE:
		printf("NONE (None)\n");
		break;
	case ET_REL:
		printf("REL (Relocatable file)\n");
		break;
	case ET_EXEC:
		printf("EXEC (Executable file)\n");
		break;
	case ET_DYN:
		printf("DYN (Shared object file)\n");
		break;
	case ET_CORE:
		printf("CORE (Core file)\n");
		break;
	default:
		printf("<unknown: %x>\n", new_type);
	}
}

/**
 * print_entry - Prints the entry point of an ELF header.
 * @new_entry: The address of the ELF entry point.
 * @new_ident: A pointer to an array containing the ELF class.
 */
void print_entry(unsigned long int new_entry, unsigned char *new_ident)
{
	printf("  Entry point address:               ");

	if (new_ident[EI_DATA] == ELFDATA2MSB)
	{
		new_entry = ((new_entry << 8) & 0xFF00FF00) |
			  ((new_entry >> 8) & 0xFF00FF);
		new_entry = (new_entry << 16) | (new_entry >> 16);
	}

	if (new_ident[EI_CLASS] == ELFCLASS32)
		printf("%#x\n", (unsigned int)new_entry);

	else
		printf("%#lx\n", new_entry);
}

/**
 * close_elf - Closes an ELF file.
 * @new_elf: The file descriptor of the ELF file.
 *
 * Description: If the file cannot be closed - exit code 98.
 */
void close_elf(int
