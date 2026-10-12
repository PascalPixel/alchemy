SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/deri_mura/entry.o"(.text)
        "*/games/tla/src/field/deri_mura/scene_tables.o"(.text)
        "*/games/tla/src/field/deri_mura/places.o"(.text)
        "*/games/tla/src/field/deri_mura/talk.o"(.text)
        "*/games/tla/src/field/deri_mura/hip.o"(.text)
        "*/resource_651_overlay.o"(.text.x020083a0)
        "*/games/tla/src/field/deri_mura/events.o"(.text)
        "*/games/tla/src/field/deri_mura/scene.o"(.text)
        "*/games/tla/src/field/deri_mura/import.o"(.text)
        "*/games/tla/src/field/deri_mura/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
