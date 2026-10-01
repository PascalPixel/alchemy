/* resource_375: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_SUKURETA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_SUKURETA/SUKURETA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_SUKURETA/IMPORT.o"(.text)
        "*/resource_375_overlay.o"(.rodata.x02009bdc)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
