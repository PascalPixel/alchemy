/* resource_3ad: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_DOU/ENTRY.o"(.text)
        "*/resource_3ad_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_DOU/IMPORT.o"(.text)
        "*/resource_3ad_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
