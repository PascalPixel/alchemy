SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/MAP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/BLACKORB.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/STORY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/LINKED_EFFECTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/IMPORT.o"(.text)
        "*/resource_371_overlay.o"(.rodata.x0200c4ac)
    }
    .bss (NOLOAD) :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/BLACKORB.o"(.bss COMMON)
        "*/games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/STORY.o"(.bss COMMON)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
