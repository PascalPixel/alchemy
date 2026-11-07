SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/osenia_cliffs/entry.o"(.text)
        "*/games/tla/src/scene/osenia_cliffs/scene_tables.o"(.text)
        "*/recon/tla/raw/src/scene/osenia_cliffs/s0054.o"(.text)
        "*/games/tla/src/scene/osenia_cliffs/scene_events.o"(.text)
        "*/games/tla/src/scene/osenia_cliffs/s0074.o"(.text)
        "*/recon/tla/raw/src/scene/osenia_cliffs/s00fc.o"(.text)
        "*/games/tla/src/scene/osenia_cliffs/prepare.o"(.text)
        "*/games/tla/src/scene/osenia_cliffs/import.o"(.text)
        "*/games/tla/src/scene/osenia_cliffs/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
