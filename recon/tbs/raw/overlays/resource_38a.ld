/* resource_38a: its listing with maintained code linked in place,
   as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_38a_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/resource_38a_overlay.o"(.text.x0200813c)
        "*/resource_38a_overlay.o"(.rodata.part1)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
