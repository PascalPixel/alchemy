/* resource_36f: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/MENU/TITLE/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/TITLE/TABLES.o"(.text)
        "*/resource_36f_overlay.o"(.text.x02008054)
        "*/games/THE BROKEN SEAL/SRC/MENU/TITLE/IMPORT.o"(.text)
        "*/resource_36f_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
