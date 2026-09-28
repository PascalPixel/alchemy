/* resource_3cb: its listing with the compiler library linked in place from
   the licensed container, as pret links libgcc members. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_3cb_overlay.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_3cb_overlay.o"(.rodata.part1)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
