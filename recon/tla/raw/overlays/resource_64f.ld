SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/suhalla_gate/entry.o"(.text)
        "*/games/tla/src/scene/suhalla_gate/events_a.o"(.text)
        "*/games/tla/src/scene/suhalla_gate/s0044.o"(.text)
        "*/games/tla/src/scene/suhalla_gate/scene_tables.o"(.text)
        "*/games/tla/src/scene/suhalla_gate/s0090.o"(.text)
        "*/games/tla/src/scene/suhalla_gate/events_b.o"(.text)
        "*/games/tla/src/scene/suhalla_gate/s028c.o"(.text)
        "*/games/tla/src/scene/suhalla_gate/prepare.o"(.text)
        "*/games/tla/src/scene/suhalla_gate/s03ac.o"(.text)
        "*/resource_64f_overlay.o"(.text.x02008494)
        "*/games/tla/src/scene/suhalla_gate/s05e8.o"(.text)
        "*/games/tla/src/scene/suhalla_gate/events_c.o"(.text)
        "*/games/tla/src/scene/suhalla_gate/import.o"(.text)
        "*/games/tla/src/scene/suhalla_gate/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
