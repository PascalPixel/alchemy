SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/islet_cave/entry.o"(.text)
        "*/games/tla/src/scene/islet_cave/s0038.o"(.text)
        "*/games/tla/src/scene/islet_cave/s0158.o"(.text)
        "*/resource_675_overlay.o"(.text.x020081d4)
        "*/resource_675_overlay.o"(.text.x020081f8)
        "*/games/tla/src/scene/islet_cave/s0328.o"(.text)
        "*/games/tla/src/scene/islet_cave/events2.o"(.text)
        "*/games/tla/src/scene/islet_cave/s03d4.o"(.text)
        "*/games/tla/src/scene/islet_cave/scene_tables.o"(.text)
        "*/games/tla/src/scene/islet_cave/s042c.o"(.text)
        "*/games/tla/src/scene/islet_cave/s04ac.o"(.text)
        "*/resource_675_overlay.o"(.text.x02008580)
        "*/resource_675_overlay.o"(.text.x020085fc)
        "*/resource_675_overlay.o"(.text.x0200869c)
        "*/games/tla/src/scene/islet_cave/prepare.o"(.text)
        "*/games/tla/src/scene/islet_cave/s07dc.o"(.text)
        "*/games/tla/src/scene/islet_cave/events.o"(.text)
        "*/resource_675_overlay.o"(.text.x020089f8)
        "*/games/tla/src/scene/islet_cave/events3.o"(.text)
        "*/games/tla/src/scene/islet_cave/s10ac.o"(.text)
        "*/games/tla/src/scene/islet_cave/s115c.o"(.text)
        "*/games/tla/src/scene/islet_cave/import.o"(.text)
        "*/games/tla/src/scene/islet_cave/data.o"(.data)
    }
    .bss : { "*/resource_675_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
