/* resource_3a1: its listing with maintained code
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_3a1_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SHIAN_HEYA/OBJECT_SPAWN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/resource_3a1_overlay.o"(.text.x0200813c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SHIAN_HEYA/FLAGGED_OBJECT.o"(.text)
        "*/resource_3a1_overlay.o"(.text.x02008504)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SHIAN_HEYA/SCENE_SETUP.o"(.text)
        "*/resource_3a1_overlay.o"(.text.x02008614)
        "*/resource_3a1_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
