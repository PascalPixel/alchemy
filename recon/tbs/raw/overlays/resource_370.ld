/* resource_370: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/MENU/CLEAR/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/CLEAR/CLEAR.o"(.text)
        "*/resource_370_overlay.o"(.text.x020083cc)
        "*/games/THE BROKEN SEAL/SRC/MENU/CLEAR/CLEAR2.o"(.text)
        "*/resource_370_overlay.o"(.text.x02008de4)
        "*/games/THE BROKEN SEAL/SRC/MENU/CLEAR/CLEAR3.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/CLEAR/IMPORT.o"(.text)
        "*/resource_370_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
