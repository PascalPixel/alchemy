/* resource_3b0: its listing with maintained code linked in place, as pret
   links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
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
        "*/resource_3b0_overlay.o"(.text.x020091b4)
        "*/resource_3b0_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
