/* resource_3c0: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_SABAKU/ENTRY.o"(.text)
        "*/resource_3c0_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_SABAKU/IMPORT.o"(.text)
        "*/resource_3c0_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
