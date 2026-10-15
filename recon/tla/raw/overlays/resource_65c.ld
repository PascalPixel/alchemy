SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/osenia_cliffs/entry.o"(.text)
        "*/games/tla/src/scene/osenia_cliffs/scene_tables.o"(.text)
        "*/games/tla/src/scene/osenia_cliffs/s0054.o"(.text)
        "*/games/tla/src/scene/osenia_cliffs/scene_events.o"(.text)
        "*/games/tla/src/scene/osenia_cliffs/s0074.o"(.text)
        "*/resource_65c_overlay.o"(.text.x020080fc)
        "*/games/tla/src/scene/osenia_cliffs/prepare.o"(.text)
        "*/games/tla/src/scene/osenia_cliffs/import.o"(.text)
        "*/games/tla/src/scene/osenia_cliffs/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
