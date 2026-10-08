SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/deri_heya/entry.o"(.text)
        "*/games/tla/src/field/deri_heya/scene_tables.o"(.text)
        "*/games/tla/src/field/deri_heya/lines.o"(.text)
        "*/games/tla/src/field/deri_heya/murabito.o"(.text)
        "*/resource_652_overlay.o"(.text.x02008704)
        "*/games/tla/src/field/deri_heya/item.o"(.text)
        "*/resource_652_overlay.o"(.text.x02008878)
        "*/games/tla/src/field/deri_heya/touchaku.o"(.text)
        "*/games/tla/src/field/deri_heya/import.o"(.text)
        "*/resource_652_overlay.o"(.rodata.x02008e04)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
