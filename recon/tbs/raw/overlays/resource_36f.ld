SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/MENU/TITLE/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/TITLE/TITLE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/TITLE/IMPORT.o"(.text)
        "*/resource_36f_overlay.o"(.rodata.x020085f8)
    }
    .bss (NOLOAD) :
    {
        "*/resource_36f_overlay.o"(.bss)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
