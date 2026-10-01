/* resource_379: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_FUNKA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_FUNKA/CONDITIONAL_SETUP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_FUNKA/IMPORT.o"(.text)
        "*/resource_379_overlay.o"(.rodata.x02008c00)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
