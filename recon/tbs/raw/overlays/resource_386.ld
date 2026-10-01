/* resource_386: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA_SAI/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA_SAI/PLACEMENT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_HEYA_SAI/IMPORT.o"(.text)
        "*/resource_386_overlay.o"(.rodata.x020086dc)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
