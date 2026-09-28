/* resource_650: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/IDEJIMA/ENTRY.o"(.text)
        "*/resource_650_overlay.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/IDEJIMA/IMPORT.o"(.text)
        "*/resource_650_overlay.o"(.text.x0200b330)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
