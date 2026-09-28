/* resource_38e: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_NIWA/ENTRY.o"(.text)
        "*/resource_38e_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_NIWA/IMPORT.o"(.text)
        "*/resource_38e_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
