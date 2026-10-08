SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_DOU/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_DOU/DOU.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_DOU/IMPORT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_DOU/DATA.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
