SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/debug/level/entry.o"(.text)
        "*/games/tla/src/debug/level/scene_tables.o"(.text)
        "*/games/tla/src/debug/level/s004c.o"(.text)
        "*/games/tla/src/debug/level/s0098.o"(.text)
        "*/games/tla/src/debug/level/s00fc.o"(.text)
        "*/games/tla/src/debug/level/level.o"(.text)
        "*/resource_6b7_overlay.o"(.text.x0200817c)
        "*/games/tla/src/debug/level/talk.o"(.text)
        "*/resource_6b7_overlay.o"(.text.x020082cc)
        "*/games/tla/src/debug/level/finish.o"(.text)
        "*/games/tla/src/debug/level/s0590.o"(.text)
        "*/games/tla/src/debug/level/pose.o"(.text)
        "*/games/tla/src/debug/level/prepare.o"(.text)
        "*/games/tla/src/debug/level/calls.o"(.text)
        "*/games/tla/src/debug/level/import.o"(.text)
        "*/games/tla/src/debug/level/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
