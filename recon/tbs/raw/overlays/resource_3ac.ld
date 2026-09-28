/* resource_3ac: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/RUNPA_SUHARA/ENTRY.o"(.text)
        "*/resource_3ac_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/RUNPA_SUHARA/IMPORT.o"(.text)
        "*/resource_3ac_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
