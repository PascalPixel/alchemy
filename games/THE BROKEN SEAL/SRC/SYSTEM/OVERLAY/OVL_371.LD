SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/MAP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/LINKED_EFFECTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/IMPORT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/GRAPHICS/WORLD_MAP/STORY.o"(.rodata)
        "*/games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/DATA.o"(.data)
    }
    .bss (NOLOAD) :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/MAP.o"(.bss COMMON)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
