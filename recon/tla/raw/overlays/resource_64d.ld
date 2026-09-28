/* resource_64d: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/VINASU_IRIGUCHI/ENTRY.o"(.text)
        "*/resource_64d_overlay.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/VINASU_IRIGUCHI/IMPORT.o"(.text)
        "*/resource_64d_overlay.o"(.text.x0200aa6c)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
