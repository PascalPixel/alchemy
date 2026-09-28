/* resource_3b0: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HOBASHIRA/ENTRY.o"(.text)
        "*/resource_3b0_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HOBASHIRA/FACING.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HOBASHIRA/ACTOR_SWAY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HOBASHIRA/TABLES.o"(.text)
        "*/resource_3b0_overlay.o"(.text.x02008240)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HOBASHIRA/SCENES.o"(.text)
        "*/resource_3b0_overlay.o"(.text.x02008564)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HOBASHIRA/OBJECT_SCALE.o"(.text)
        "*/resource_3b0_overlay.o"(.text.x02008e78)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HOBASHIRA/VALUES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HOBASHIRA/IMPORT.o"(.text)
        "*/resource_3b0_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
