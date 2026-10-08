SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_FUNKA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_FUNKA/CONDITIONAL_SETUP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_FUNKA/IMPORT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_FUNKA/DATA.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
