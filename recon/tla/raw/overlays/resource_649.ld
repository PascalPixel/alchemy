SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_649/entry.o"(.text)
        "*/games/tla/src/field/overlay_649/scene_tables.o"(.text)
        "*/resource_649_overlay.o"(.text.x0200805c)
        "*/games/tla/src/field/overlay_649/prepare.o"(.text)
        "*/resource_649_overlay.o"(.text.x02008278)
        "*/resource_649_overlay.o"(.text.x0200831c)
        "*/resource_649_overlay.o"(.text.x02008490)
        "*/games/tla/src/field/overlay_649/import.o"(.text)
        "*/resource_649_overlay.o"(.rodata.x0200864c)
    }
    .bss : { "*/resource_649_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
