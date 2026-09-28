/* resource_3ae: its listing with maintained code linked in place,
   as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_3ae_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KAREI_TOREBI/FLAGGED_ACTORS.o"(.text)
        "*/resource_3ae_overlay.o"(.text.x02008ad4)
        "*/resource_3ae_overlay.o"(.rodata.part1)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
