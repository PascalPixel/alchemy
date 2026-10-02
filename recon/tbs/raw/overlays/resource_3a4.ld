/* resource_3a4: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/YAMA.o"(.text)
        "*/resource_3a4_overlay.o"(.text.x02008d2c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/YAMA2.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/YAMA3.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/IMPORT.o"(.text)
        "*/resource_3a4_overlay.o"(.rodata.x0200bd28)
    }
    .bss : { "*/resource_3a4_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
