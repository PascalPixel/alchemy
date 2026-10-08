SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_HEYA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_HEYA/ENTRY_LAYOUT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_HEYA/IMPORT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_HEYA/DATA.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
