/* resource_3be: its listing with the compiler library linked in place from
   the licensed container, as pret links libgcc members. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2000000 :
    {
        "*/resource_3be_overlay.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_3be_overlay.o"(.text.part1)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
