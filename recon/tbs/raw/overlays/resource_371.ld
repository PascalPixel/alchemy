/* resource_371: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/ENTRY.o"(.text)
        "*/resource_371_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/LINKED_EFFECTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/IMPORT.o"(.text)
        "*/resource_371_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
