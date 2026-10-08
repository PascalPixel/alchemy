SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/world_map/entry.o"(.text)
        "*/games/tbs/src/field/world_map/map.o"(.text)
        "*/games/tbs/src/field/world_map/linked_effects.o"(.text)
        "*/games/tbs/src/field/world_map/import.o"(.text)
        "*/games/tbs/src/graphics/world_map/story.o"(.rodata)
        "*/games/tbs/src/field/world_map/data.o"(.data)
    }
    .bss (NOLOAD) :
    {
        "*/games/tbs/src/field/world_map/map.o"(.bss COMMON)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
