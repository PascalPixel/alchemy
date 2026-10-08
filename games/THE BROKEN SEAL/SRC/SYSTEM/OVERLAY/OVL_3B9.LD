SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORASHIAMU_IRIGUCHI/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORASHIAMU_IRIGUCHI/IRIGUCHI.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORASHIAMU_IRIGUCHI/IMPORT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORASHIAMU_IRIGUCHI/DATA.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
