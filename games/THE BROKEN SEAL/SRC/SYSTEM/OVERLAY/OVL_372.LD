SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_ARASHI/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_ARASHI/ARASHI.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_ARASHI/IMPORT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_ARASHI/DATA.o"(.data)
    }
    .bss (NOLOAD) :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_ARASHI/ARASHI.o"(.bss COMMON)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
