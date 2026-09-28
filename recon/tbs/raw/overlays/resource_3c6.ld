/* resource_3c6: its listing with maintained code linked in place, as pret
   links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_3c6_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_MACHI/SCENE_TABLES.o"(.text)
        "*/resource_3c6_overlay.o"(.text.x020080c4)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_MACHI/FLAGGED_CUE.o"(.text)
        "*/resource_3c6_overlay.o"(.text.x020094d8)
        "*/resource_3c6_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
