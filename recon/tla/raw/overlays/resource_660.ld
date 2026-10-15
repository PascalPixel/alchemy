SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_660/entry.o"(.text)
        "*/games/tla/src/field/overlay_660/scene_tables.o"(.text)
        "*/games/tla/src/field/overlay_660/s004c.o"(.text)
        "*/games/tla/src/field/overlay_660/s039c.o"(.text)
        "*/games/tla/src/field/overlay_660/s0580.o"(.text)
        "*/games/tla/src/field/overlay_660/prepare.o"(.text)
        "*/games/tla/src/field/overlay_660/s0710.o"(.text)
        "*/games/tla/src/field/overlay_660/import.o"(.text)
        "*/games/tla/src/field/overlay_660/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
