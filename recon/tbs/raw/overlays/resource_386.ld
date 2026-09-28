/* resource_386: its listing with maintained code linked in place, as pret
   links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_386_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA_SAI/PLACEMENT.o"(.text)
        "*/resource_386_overlay.o"(.text.x020085dc)
        "*/resource_386_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
