SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/DEBUG/MENU_TEST/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/DEBUG/MENU_TEST/TEST.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/DEBUG/MENU_TEST/IMPORT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/DEBUG/MENU_TEST/DATA.o"(.data)
    }
    .bss : { "*/resource_3ce_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
