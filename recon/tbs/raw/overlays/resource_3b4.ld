/* resource_3b4: its listing with maintained code and the compiler library
   linked in place, as pret links objects and libgcc members in ROM order. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_3b4_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/OBJECT/STAGED_ACTOR.o"(.text)
        "*/resource_3b4_overlay.o"(.text.x020089dc)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_3b4_overlay.o"(.rodata.part1)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
