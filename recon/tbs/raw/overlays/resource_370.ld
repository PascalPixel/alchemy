SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/MENU/CLEAR/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/CLEAR/CLEAR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/CLEAR/TILEGRID.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/CLEAR/PASSWORD.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/CLEAR/IMPORT.o"(.text)
        "*/resource_370_overlay.o"(.rodata.x020094d4)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
