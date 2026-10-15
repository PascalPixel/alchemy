SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_6b7/entry.o"(.text)
        "*/games/tla/src/field/overlay_6b7/scene_tables.o"(.text)
        "*/games/tla/src/field/overlay_6b7/s004c.o"(.text)
        "*/games/tla/src/field/overlay_6b7/s0098.o"(.text)
        "*/games/tla/src/field/overlay_6b7/s00fc.o"(.text)
        "*/games/tla/src/field/overlay_6b7/level.o"(.text)
        "*/resource_6b7_overlay.o"(.text.x0200817c)
        "*/games/tla/src/field/overlay_6b7/talk.o"(.text)
        "*/resource_6b7_overlay.o"(.text.x020082cc)
        "*/games/tla/src/field/overlay_6b7/finish.o"(.text)
        "*/games/tla/src/field/overlay_6b7/s0590.o"(.text)
        "*/games/tla/src/field/overlay_6b7/pose.o"(.text)
        "*/games/tla/src/field/overlay_6b7/prepare.o"(.text)
        "*/games/tla/src/field/overlay_6b7/calls.o"(.text)
        "*/games/tla/src/field/overlay_6b7/import.o"(.text)
        "*/games/tla/src/field/overlay_6b7/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
