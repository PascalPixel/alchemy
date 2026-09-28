/* resource_38d: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_KYUDEN/ENTRY.o"(.text)
        "*/resource_38d_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_KYUDEN/IMPORT.o"(.text)
        "*/resource_38d_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
