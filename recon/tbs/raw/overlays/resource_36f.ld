/* resource_36f: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. The overlay's own work follows the image. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/MENU/TITLE/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/TITLE/TITLE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/TITLE/IMPORT.o"(.text)
        "*/resource_36f_overlay.o"(.rodata)
    }
    .bss (NOLOAD) :
    {
        "*/resource_36f_overlay.o"(.bss)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
