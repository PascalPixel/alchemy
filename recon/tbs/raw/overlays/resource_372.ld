/* resource_372: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. The overlay's own work follows the image. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_ARASHI/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_ARASHI/ARASHI.o"(.text)
        "*/resource_372_overlay.o"(.text.x0200b1ac)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_ARASHI/ARASHI2.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_ARASHI/IMPORT.o"(.text)
        "*/resource_372_overlay.o"(.rodata)
    }
    .bss (NOLOAD) :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_ARASHI/ARASHI.o"(.bss COMMON)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
