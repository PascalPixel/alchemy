/* resource_390: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_HEYA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_HEYA/ENTRY_LAYOUT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_HEYA/IMPORT.o"(.text)
        "*/resource_390_overlay.o"(.rodata.x02008318)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
