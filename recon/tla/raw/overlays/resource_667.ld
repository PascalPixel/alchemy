SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_667/entry.o"(.text)
        "*/games/tla/src/field/overlay_667/scene_tables.o"(.text)
        "*/games/tla/src/field/overlay_667/talk.o"(.text)
        "*/games/tla/src/field/overlay_667/scene_events.o"(.text)
        "*/resource_667_overlay.o"(.text.x02008514)
        "*/games/tla/src/field/overlay_667/prepare.o"(.text)
        "*/resource_667_overlay.o"(.text.x02008658)
        "*/games/tla/src/field/overlay_667/update.o"(.text)
        "*/games/tla/src/field/overlay_667/import.o"(.text)
        "*/games/tla/src/field/overlay_667/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
