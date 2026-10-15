SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_693/entry.o"(.text)
        "*/games/tla/src/field/overlay_693/scene_tables.o"(.text)
        "*/games/tla/src/field/overlay_693/s0064.o"(.text)
        "*/games/tla/src/field/overlay_693/s00e0.o"(.text)
        "*/games/tla/src/field/overlay_693/s0120.o"(.text)
        "*/games/tla/src/field/overlay_693/s0208.o"(.text)
        "*/games/tla/src/field/overlay_693/s03e4.o"(.text)
        "*/resource_693_overlay.o"(.text.x02008640)
        "*/games/tla/src/field/overlay_693/gate.o"(.text)
        "*/games/tla/src/field/overlay_693/s097c.o"(.text)
        "*/games/tla/src/field/overlay_693/variant.o"(.text)
        "*/games/tla/src/field/overlay_693/s0c14.o"(.text)
        "*/games/tla/src/field/overlay_693/s0c8c.o"(.text)
        "*/games/tla/src/field/overlay_693/s0d08.o"(.text)
        "*/games/tla/src/field/overlay_693/s0d3c.o"(.text)
        "*/games/tla/src/field/overlay_693/s0dc4.o"(.text)
        "*/games/tla/src/field/overlay_693/import.o"(.text)
        "*/games/tla/src/field/overlay_693/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
