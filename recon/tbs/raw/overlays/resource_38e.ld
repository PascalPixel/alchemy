/* resource_38e: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_NIWA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_NIWA/NIWA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_NIWA/IMPORT.o"(.text)
        "*/resource_38e_overlay.o"(.rodata.x02008bd4)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
