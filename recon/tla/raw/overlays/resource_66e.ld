SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_66e/entry.o"(.text)
        "*/games/tla/src/field/overlay_66e/scene_tables.o"(.text)
        "*/games/tla/src/field/overlay_66e/script0.o"(.text)
        "*/games/tla/src/field/overlay_66e/people.o"(.text)
        "*/resource_66e_overlay.o"(.text.x020080c4)
        "*/resource_66e_overlay.o"(.text.x02008124)
        "*/resource_66e_overlay.o"(.text.x02008184)
        "*/resource_66e_overlay.o"(.text.x020081e4)
        "*/resource_66e_overlay.o"(.text.x02008244)
        "*/resource_66e_overlay.o"(.text.x020082a4)
        "*/games/tla/src/field/overlay_66e/fortune.o"(.text)
        "*/resource_66e_overlay.o"(.text.x02008390)
        "*/games/tla/src/field/overlay_66e/reading.o"(.text)
        "*/resource_66e_overlay.o"(.text.x02008b60)
        "*/resource_66e_overlay.o"(.text.x02008ba0)
        "*/resource_66e_overlay.o"(.text.x02008c04)
        "*/resource_66e_overlay.o"(.text.x02008f28)
        "*/games/tla/src/field/overlay_66e/prepare.o"(.text)
        "*/games/tla/src/field/overlay_66e/import.o"(.text)
        "*/resource_66e_overlay.o"(.rodata.x020091b0)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
