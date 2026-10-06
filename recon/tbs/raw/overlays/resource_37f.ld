SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_IRIGUCHI/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_IRIGUCHI/IRIGUCHI.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_IRIGUCHI/IMPORT.o"(.text)
        "*/resource_37f_overlay.o"(.rodata.x02009cd4)
    }
    .bss (NOLOAD) :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_IRIGUCHI/IRIGUCHI.o"(.bss)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
