/* resource_375: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_SUKURETA/ENTRY.o"(.text)
        "*/resource_375_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_SUKURETA/IMPORT.o"(.text)
        "*/resource_375_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
