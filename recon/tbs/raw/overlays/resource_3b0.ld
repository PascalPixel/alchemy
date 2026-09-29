/* resource_3b0: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. The overlay's own work follows the image. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HOBASHIRA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HOBASHIRA/CAMERA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HOBASHIRA/FACING.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HOBASHIRA/ACTOR_SWAY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HOBASHIRA/TABLES.o"(.text)
        "*/resource_3b0_overlay.o"(.text.x02008240)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HOBASHIRA/SCENES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HOBASHIRA/STAGING.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HOBASHIRA/OBJECT_SCALE.o"(.text)
        "*/resource_3b0_overlay.o"(.text.x02008e78)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HOBASHIRA/SWAY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HOBASHIRA/VALUES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HOBASHIRA/IMPORT.o"(.text)
        "*/resource_3b0_overlay.o"(.rodata)
    }
    .bss ALIGN(16) (NOLOAD) :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HOBASHIRA/SWAY.o"(.bss)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
