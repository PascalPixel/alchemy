SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/alhafran_cave/entry.o"(.text)
        "*/games/tla/src/scene/alhafran_cave/scene_tables.o"(.text)
        "*/games/tla/src/scene/alhafran_cave/s004c.o"(.text)
        "*/resource_66a_overlay.o"(.text.x020082f8)
        "*/games/tla/src/scene/alhafran_cave/s07a4.o"(.text)
        "*/resource_66a_overlay.o"(.text.x02008818)
        "*/games/tla/src/scene/alhafran_cave/s1220.o"(.text)
        "*/games/tla/src/scene/alhafran_cave/s1230.o"(.text)
        "*/resource_66a_overlay.o"(.text.x0200929c)
        "*/games/tla/src/scene/alhafran_cave/s1320.o"(.text)
        "*/games/tla/src/scene/alhafran_cave/import.o"(.text)
        "*/games/tla/src/scene/alhafran_cave/data.o"(.data)
    }
    .bss : { "*/resource_66a_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
