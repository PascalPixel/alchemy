/* resource_37e: its listing with maintained code
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_37e_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_ROKA/SCENE_37E.o"(.text)
        "*/resource_37e_overlay.o"(.text.x020080a0)
        "*/resource_37e_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
