SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_66a/entry.o"(.text)
        "*/games/tla/src/field/overlay_66a/scene_tables.o"(.text)
        "*/games/tla/src/field/overlay_66a/s004c.o"(.text)
        "*/resource_66a_overlay.o"(.text.x020082f8)
        "*/resource_66a_overlay.o"(.text.x0200851c)
        "*/games/tla/src/field/overlay_66a/s07a4.o"(.text)
        "*/resource_66a_overlay.o"(.text.x02008818)
        "*/games/tla/src/field/overlay_66a/s1220.o"(.text)
        "*/resource_66a_overlay.o"(.text.x02009230)
        "*/resource_66a_overlay.o"(.text.x0200929c)
        "*/resource_66a_overlay.o"(.text.x02009320)
        "*/games/tla/src/field/overlay_66a/import.o"(.text)
        "*/games/tla/src/field/overlay_66a/data.o"(.data)
    }
    .bss : { "*/resource_66a_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
