/* resource_3cd: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/DEBUG/ITEM_LEVEL/ENTRY.o"(.text)
        "*/resource_3cd_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/DEBUG/ITEM_LEVEL/IMPORT.o"(.text)
        "*/resource_3cd_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
