/* resource_64f: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/SUHARA_GATE/ENTRY.o"(.text)
        "*/resource_64f_overlay.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/SUHARA_GATE/IMPORT.o"(.text)
        "*/resource_64f_overlay.o"(.rodata.x02008aa8)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
