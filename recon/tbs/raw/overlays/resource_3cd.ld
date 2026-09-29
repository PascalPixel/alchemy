/* resource_3cd: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/DEBUG/ITEM_LEVEL/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/DEBUG/ITEM_LEVEL/RECORD_COUNT.o"(.text)
        "*/resource_3cd_overlay.o"(.text.x020080ec)
        "*/games/THE BROKEN SEAL/SRC/DEBUG/ITEM_LEVEL/SETUP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/DEBUG/ITEM_LEVEL/IMPORT.o"(.text)
        "*/resource_3cd_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
