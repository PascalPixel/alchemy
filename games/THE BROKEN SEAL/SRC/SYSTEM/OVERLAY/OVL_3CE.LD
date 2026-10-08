SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/DEBUG/MENU_TEST/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/DEBUG/MENU_TEST/TEST.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/DEBUG/MENU_TEST/IMPORT.o"(.text)
        "*/resource_3ce_overlay.o"(.rodata.x020092b0)
    }
    .bss : { "*/resource_3ce_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
