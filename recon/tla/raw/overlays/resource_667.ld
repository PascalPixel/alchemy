SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/alhafra_manor/entry.o"(.text)
        "*/games/tla/src/scene/alhafra_manor/scene_tables.o"(.text)
        "*/games/tla/src/scene/alhafra_manor/talk.o"(.text)
        "*/games/tla/src/scene/alhafra_manor/scene_events.o"(.text)
        "*/resource_667_overlay.o"(.text.x02008514)
        "*/games/tla/src/scene/alhafra_manor/prepare.o"(.text)
        "*/resource_667_overlay.o"(.text.x02008658)
        "*/games/tla/src/scene/alhafra_manor/update.o"(.text)
        "*/games/tla/src/scene/alhafra_manor/import.o"(.text)
        "*/games/tla/src/scene/alhafra_manor/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
