/* resource_39b: its listing with the compiler library linked in place from
   the licensed container, as pret links libgcc members. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2000000 :
    {
        "*/resource_39b_overlay.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_39b_overlay.o"(.text.part1)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
