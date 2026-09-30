/* resource_3ad: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code, and the cave's scene tables follow them. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_DOU/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_DOU/DOU.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_DOU/IMPORT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_DOU/DATA.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
