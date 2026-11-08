SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/scene/atteka_inlet/entry.o"(.text)
        "*/games/tla/src/scene/atteka_inlet/scene_tables.o"(.text)
        "*/games/tla/src/scene/atteka_inlet/s0064.o"(.text)
        "*/games/tla/src/scene/atteka_inlet/s00e0.o"(.text)
        "*/games/tla/src/scene/atteka_inlet/s0120.o"(.text)
        "*/games/tla/src/scene/atteka_inlet/s0208.o"(.text)
        "*/games/tla/src/scene/atteka_inlet/s03e4.o"(.text)
        "*/games/tla/src/scene/atteka_inlet/s0640.o"(.text)
        "*/games/tla/src/scene/atteka_inlet/gate.o"(.text)
        "*/games/tla/src/scene/atteka_inlet/s097c.o"(.text)
        "*/games/tla/src/scene/atteka_inlet/variant.o"(.text)
        "*/games/tla/src/scene/atteka_inlet/s0c14.o"(.text)
        "*/games/tla/src/scene/atteka_inlet/s0c8c.o"(.text)
        "*/games/tla/src/scene/atteka_inlet/s0d08.o"(.text)
        "*/games/tla/src/scene/atteka_inlet/s0d3c.o"(.text)
        "*/games/tla/src/scene/atteka_inlet/s0dc4.o"(.text)
        "*/games/tla/src/scene/atteka_inlet/import.o"(.text)
        "*/games/tla/src/scene/atteka_inlet/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
