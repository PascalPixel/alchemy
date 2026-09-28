/* resource_392: its listing with maintained code and the compiler library
   linked in place, as pret links objects and libgcc members in ROM order. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_392_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/OBJECT/STAGED_ACTOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_HIROBA/STAGED_PARTICLE.o"(.text)
        "*/resource_392_overlay.o"(.text.x02008b8c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_HIROBA/ORBIT.o"(.text)
        "*/resource_392_overlay.o"(.text.x02008d8c)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_392_overlay.o"(.rodata.part1)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
