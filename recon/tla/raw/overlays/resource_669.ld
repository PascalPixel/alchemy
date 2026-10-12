SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_669/entry.o"(.text)
        "*/games/tla/src/field/overlay_669/scene_tables.o"(.text)
        "*/games/tla/src/field/overlay_669/s0058.o"(.text)
        "*/games/tla/src/field/overlay_669/events.o"(.text)
        "*/games/tla/src/field/overlay_669/scene_events.o"(.text)
        "*/resource_669_overlay.o"(.text.x02008250)
        "*/games/tla/src/field/overlay_669/prepare.o"(.text)
        "*/resource_669_overlay.o"(.text.x020083e8)
        "*/resource_669_overlay.o"(.text.x02008438)
        "*/games/tla/src/field/overlay_669/cutscene.o"(.text)
        "*/games/tla/src/field/overlay_669/import.o"(.text)
        "*/games/tla/src/field/overlay_669/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
