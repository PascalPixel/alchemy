/* resource_395: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_KI/ENTRY.o"(.text)
        "*/resource_395_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_KI/GESTURE.o"(.text)
        "*/resource_395_overlay.o"(.text.x0200952c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_KI/IMPORT.o"(.text)
        "*/resource_395_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
