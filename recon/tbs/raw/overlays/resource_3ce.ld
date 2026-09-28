/* resource_3ce: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/DEBUG/MENU_TEST/ENTRY.o"(.text)
        "*/resource_3ce_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/DEBUG/MENU_TEST/IMPORT.o"(.text)
        "*/resource_3ce_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
