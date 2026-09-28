/* resource_3ba: its listing with maintained code and the compiler library
   linked in place, as pret links objects and libgcc members in ROM order. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_3ba_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KOROSSEO/ITEM_ICON.o"(.text)
        "*/resource_3ba_overlay.o"(.text.x0200af64)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KOROSSEO/PATH_RIVAL.o"(.text)
        "*/resource_3ba_overlay.o"(.text.x0200b3a0)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_3ba_overlay.o"(.rodata.part1)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
