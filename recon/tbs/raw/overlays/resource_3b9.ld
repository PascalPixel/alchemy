SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORASHIAMU_IRIGUCHI/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORASHIAMU_IRIGUCHI/IRIGUCHI.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORASHIAMU_IRIGUCHI/GATHER.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORASHIAMU_IRIGUCHI/FINALS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORASHIAMU_IRIGUCHI/IMPORT.o"(.text)
        "*/resource_3b9_overlay.o"(.rodata.x0200ad3c)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
