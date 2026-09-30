/* resource_398: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_DOU/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_DOU/DOU.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_DOU/IMPORT.o"(.text)
        "*/resource_398_overlay.o"(.rodata.x020089b0)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
