/* resource_371: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. The overlay's own work follows the image. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/MAP.o"(.text)
        "*/resource_371_overlay.o"(.text.x02009ca4)
        "*/games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/MAP2.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/MAP3.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/LINKED_EFFECTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/IMPORT.o"(.text)
        "*/resource_371_overlay.o"(.rodata.x0200c4ac)
    }
    .bss (NOLOAD) :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/MAP2.o"(.bss COMMON)
        "*/games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/MAP3.o"(.bss COMMON)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
