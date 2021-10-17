#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/mman.h>
#include <dlfcn.h>

#include "./elf.h"

// Basic print for elf header
void printElfHeader(Elf64_Ehdr *elf_header)
{
    std::cout << "\n\nElf Header\n"
              << std::endl;
    std::cout << "0x00  size: 4   ei_mag:           " << elf_header->e_ident << std::endl;
    std::cout << "0x10  size: 2   e_type:           " << elf_header->e_type << std::endl;
    std::cout << "0x12  size: 2   e_machine:        " << elf_header->e_machine << std::endl;
    std::cout << "0x14  size: 4   e_version:        " << elf_header->e_version << std::endl;
    std::cout << "0x18  size: 8   e_entry:          " << elf_header->e_entry << std::endl;
    std::cout << "0x20  size: 8   e_phoff:          " << elf_header->e_phoff << std::endl;
    std::cout << "0x28  size: 8   e_shoff:          " << elf_header->e_shoff << std::endl;
    std::cout << "0x30  size: 4   e_flags:          " << elf_header->e_flags << std::endl;
    std::cout << "0x34  size: 2   e_ehsize:         " << elf_header->e_ehsize << std::endl;
    std::cout << "0x36  size: 2   e_phentsize:      " << elf_header->e_phentsize << std::endl;
    std::cout << "0x38  size: 2   e_phnum:          " << elf_header->e_phnum << std::endl;
    std::cout << "0x3A  size: 2   e_shentsize:      " << elf_header->e_shentsize << std::endl;
    std::cout << "0x3C  size: 2   e_shnum:          " << elf_header->e_shnum << std::endl;
    std::cout << "0x3E  size: 2   e_shstrndx:       " << elf_header->e_shstrndx << std::endl;
}

// Basic print for program header
void printProgramHeader(Elf64_Phdr *program_header)
{
    std::cout << "0x00  size: 4   p_type:           " << std::hex << program_header->p_type << std::endl;
    std::cout << "0x04  size: 4   p_flags:          " << std::dec << program_header->p_flags << std::endl;
    std::cout << "0x08  size: 8   p_offset:         " << program_header->p_offset << std::endl;
    std::cout << "0x10  size: 8   p_vaddr:          " << program_header->p_vaddr << std::endl;
    std::cout << "0x18  size: 8   p_paddr:          " << program_header->p_paddr << std::endl;
    std::cout << "0x20  size: 8   p_filesz:         " << program_header->p_filesz << std::endl;
    std::cout << "0x28  size: 8   p_memsz:          " << program_header->p_memsz << std::endl;
    std::cout << "0x30  size: 8   p_align:          " << program_header->p_align << std::endl;
}

// Basic print for section header
void printSectionHeader(Elf64_Shdr *section_header, char *shstrptr)
{
    std::cout << "0x00  size: 4   sh_name:          " << (shstrptr + section_header->sh_name) << std::endl;
    std::cout << "0x04  size: 4   sh_type:          " << section_header->sh_type << std::endl;
    std::cout << "0x08  size: 8   sh_flags:         " << section_header->sh_flags << std::endl;
    std::cout << "0x10  size: 8   sh_addr:          " << section_header->sh_addr << std::endl;
    std::cout << "0x18  size: 8   sh_offset:        " << section_header->sh_offset << std::endl;
    std::cout << "0x20  size: 8   sh_size:          " << section_header->sh_size << std::endl;
    std::cout << "0x28  size: 4   sh_link:          " << section_header->sh_link << std::endl;
    std::cout << "0x2C  size: 4   sh_info:          " << section_header->sh_info << std::endl;
    std::cout << "0x30  size: 8   sh_addralign:     " << section_header->sh_addralign << std::endl;
    std::cout << "0x38  size: 8   sh_entsize:       " << section_header->sh_entsize << std::endl;
}

// Print basic file information
void parseAndPrint(char *bytes)
{
    std::cout << "64 Bit" << std::endl;

    Elf64_Ehdr *elf_header = (Elf64_Ehdr *)bytes;

    printElfHeader(elf_header);

    Elf64_Shdr *shstrHdr = (Elf64_Shdr *)(bytes + elf_header->e_shoff + (elf_header->e_shstrndx * elf_header->e_shentsize));

    char *shstrptr = (bytes + shstrHdr->sh_offset);

    for (int i = 0; i < elf_header->e_phnum; i++)
    {
        Elf64_Phdr *program_header = (Elf64_Phdr *)(bytes + elf_header->e_phoff + (i * elf_header->e_phentsize));

        std::cout << "\n\nProgram Header " << i << std::endl
                  << std::endl;
        printProgramHeader(program_header);
    }

    for (int i = 0; i < elf_header->e_shnum; i++)
    {
        Elf64_Shdr *section_header = (Elf64_Shdr *)(bytes + elf_header->e_shoff + (i * elf_header->e_shentsize));

        std::cout << "\n\nSection Header " << i << std::endl
                  << std::endl;
        printSectionHeader(section_header, shstrptr);
    }
}

const char *PHTYPE[] = {
    "PT_NULL   ", "PT_LOAD   ", "PT_DYNAMIC", "PT_INTERP ", "PT_NOTE   ",
    "PT_SHLIB  ", "PT_PHDR   ", "PT_TLS    " 
};

// Section header types
const char *SHTYPES[] = {
    "Section header table entry unused",
    "Program data",
    "Symbol table",
    "String table",
    "Relocation entries with addends",
    "Symbol hash table",
    "Dynamic linking information",
    "Notes",
    "Program space with no data (bss)",
    "Relocation entries, no addends",
    "Reserved",
    "Dynamic linker symbol table",
    "Array of constructors",
    "Array of destructors",
    "Array of pre-constructors",
    "Section group",
    "Extended section indices",
    "Number of defined types."};

// Symbols bindings
const char *BINDINGS[] {
    "Local ", "Global", "Weak  "
};

// Symbols types
const char *STTYPES[] {
    "NOTYPE", "OBJECT", "FUNC", "Section", "FILE",
    "COMMON", "TLS", "LOOS", "HIOS", "LOPROC", "SPARC_REGISTER",
    "HIPROC"
};

// Parse the symbols tab
void parseSymTab(char *bytes, Elf64_Ehdr *hdr, Elf64_Shdr *shdr, int i)
{
    // Get start to symbols
    Elf64_Sym *symbols = (Elf64_Sym *)(bytes + shdr[i].sh_offset);

    // Loop through all symbols
    for (int j = 0; j < (shdr[i].sh_size / shdr[i].sh_entsize); j++)
    {
        // If undefined
        if (symbols[j].st_shndx == SHN_UNDEF)
        {
            char *symname = (char *)(symbols[j].st_name + shdr[shdr[i].sh_link].sh_offset + bytes);
            std::cout << "\tName: " << symname;

            for (int k = 0; k + strlen(symname) < 30; k++)
                std::cout << " ";

            std::cout << "\tType: External\t";
        }
        // If absolute position
        else if (symbols[j].st_shndx == SHN_ABS)
        {
            char *symname = (char *)(symbols[j].st_name + shdr[shdr[i].sh_link].sh_offset + bytes);
            std::cout << "\tName: " << symname;

            for (int k = 0; k + strlen(symname) < 30; k++)
                std::cout << " ";

            std::cout << "\tVirtual Address: " << symbols[j].st_value;
        }
        // Anything else
        else
        {
            char *symname = (char *)(shdr[symbols[j].st_shndx].sh_name + shdr[hdr->e_shstrndx].sh_offset + bytes);
            std::cout << "\tName: " << symname;

            for (int k = 0; k + strlen(symname) < 30; k++)
                std::cout << " ";

            // Calculate virtual address from relative offset
            std::cout << "\tVirtual Address: " << shdr[symbols[j].st_shndx].sh_offset + symbols[j].st_value;
        }
        std::cout << "\t\tBinding: " << BINDINGS[ELF64_ST_BIND(symbols[j].st_info)] << "\t\tType: " << STTYPES[ELF64_ST_TYPE(symbols[j].st_info)] << std::endl;
    }
}

// Main function, first arg should be the input elf file
int main(int argc, char **argv, char **envp)
{
    // Infile to read
    std::ifstream ifs(argv[1], std::ios::binary | std::ios::ate);

    bool strings = false;
    bool symbols = false;
    bool bss = false;
    bool reloc = false;
    bool basic = false;

    // What to display
    if (argc > 2)
    {
        for (int i = 2; i < argc; i++)
        {
            if (!strcmp(argv[i], "-str"))
                strings = true;
            else if (!strcmp(argv[i], "-sym"))
                symbols = true;
            else if (!strcmp(argv[i], "-bss"))
                bss = true;
            else if (!strcmp(argv[i], "-reloc"))
                reloc = true;
            else if (!strcmp(argv[i], "-basic"))
                basic = true;
            else if (!strcmp(argv[i], "-a"))
            {
                strings = true;
                symbols = true;
                bss = true;
            }
        }
    }

    // Gets the length of the file
    int length = ifs.tellg();

    // Declares the char array
    char *bytes = new char[length];

    // Resets the seek
    ifs.seekg(0, std::ios::beg);

    // Reads all the chars
    ifs.read(bytes, length);

    if (*(bytes + 4) == 2)
    {

        if (basic)
        {
            parseAndPrint(bytes);
        }
        else
        {
            // Elf header
            Elf64_Ehdr *hdr = (Elf64_Ehdr *)bytes;

            // program and section headers
            Elf64_Phdr *phdr = (Elf64_Phdr *)(bytes + hdr->e_phoff);
            Elf64_Shdr *shdr = (Elf64_Shdr *)(bytes + hdr->e_shoff);

            std::cout << "Program Header Table\n" << std::endl;

            for (int i = 0; i < hdr->e_phnum; i++)
            {
                if (phdr[i].p_type < 8)
                    std::cout << "Type: " << PHTYPE[phdr[i].p_type];
                else
                    std::cout << "Type: " << std::hex << phdr[i].p_type << "\t";

                std::cout << "\tOffset: " << std::dec << phdr[i].p_offset << std::endl << std::endl;
                
            }

            std::cout << "\nSection Header Tables\n" << std::endl;

            for (int i = 0; i < hdr->e_shnum; i++)
            {
                // Get the name and type of each section table
                char *name = (char *)(shdr[i].sh_name + shdr[hdr->e_shstrndx].sh_offset + bytes);

                if (strlen(name))
                {
                    std::cout << "Name: " << name;

                    for (int j = 0; j + strlen(name) < 30; j++)
                        std::cout << " ";

                    if (shdr[i].sh_type < 18)
                        std::cout << "Type: " << SHTYPES[shdr[i].sh_type] << std::endl;
                    else
                        std::cout << "Type: " << shdr[i].sh_type << std::endl;
                }

                // Parse symbol table
                if (shdr[i].sh_type == SHT_SYMTAB && symbols)
                {
                    parseSymTab(bytes, hdr, shdr, i);
                }
                // Parse string table
                else if (shdr[i].sh_type == SHT_STRTAB && strings)
                {
                    char *str = (char *)(shdr[i].sh_offset + bytes);
                    while (str < (shdr[i].sh_size + shdr[i].sh_offset + bytes))
                    {
                        if (strlen(str))
                        {
                            std::cout << "\t" << str << std::endl;
                            str = (str + strlen(str));
                        }

                        while (!strlen(str) && str < (shdr[i].sh_size + shdr[i].sh_offset + bytes))
                        {
                            str++;
                        }
                    }
                }
                // Parse bss sections
                else if (shdr[i].sh_type == SHT_NOBITS && bss)
                {
                    if (!shdr[i].sh_size)
                        continue;
                    if (shdr[i].sh_flags & SHF_ALLOC)
                    {
                        std::cout << "\tAllocate " << shdr[i].sh_size << " bytes at (allocated memory - header)" << std::endl;
                        ;
                    }
                }
                // Parse rel sections
                else if (shdr[i].sh_type == SHT_REL && reloc)
                {
                    Elf64_Rel * rel = (Elf64_Rel *)(bytes + shdr[i].sh_offset);
                    for (int j = 0; j < (shdr[i].sh_size / shdr[i].sh_entsize); j++)
                    {
                        std::cout << "\tRelocation" << std::endl;
                    }
                }
                // Parse rela sections
                else if (shdr[i].sh_type == SHT_RELA && reloc)
                {
                    Elf64_Rela * rel = (Elf64_Rela *)(bytes + shdr[i].sh_offset);
                    for (int j = 0; j < (shdr[i].sh_size / shdr[i].sh_entsize); j++)
                    {
                        char * symname = (char *)(shdr[ELF64_R_SYM(rel[j].r_info)].sh_name + shdr[hdr->e_shstrndx].sh_offset + bytes);
                        std::cout << "\tOffset: " << rel[j].r_offset << "\tSymbol: " << symname;
                        for (int k = 0; k + strlen(symname) < 30; k++)
                            std::cout << " ";
                        std::cout << "Type: " << ELF64_R_TYPE(rel[j].r_info) << "\tAddend: " << rel[j].r_addend << std::endl;
                    }
                }

                std::cout << std::endl;
            }
        }
    }

    ifs.close();
}