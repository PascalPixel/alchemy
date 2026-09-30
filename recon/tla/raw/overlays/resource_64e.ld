/* resource_64e: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/VINASU_CHOJO/ENTRY.o"(.text)
        "*/resource_64e_overlay.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/VINASU_CHOJO/PUSH.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/VINASU_CHOJO/STATUE.o"(.text)
        "*/resource_64e_overlay.o"(.text.x02008a0c)
        "*/games/THE LOST AGE/SRC/FIELD/VINASU_CHOJO/IMPORT.o"(.text)
        "*/resource_64e_overlay.o"(.rodata.x02009ce4)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
