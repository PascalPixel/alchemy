/* resource_651: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/DERI_MURA/ENTRY.o"(.text)
        "*/resource_651_overlay.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/DERI_MURA/IMPORT.o"(.text)
        "*/resource_651_overlay.o"(.text.x02008d80)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
