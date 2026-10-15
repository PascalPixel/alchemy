SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/debug/psynergy_test/entry.o"(.text)
        "*/games/tla/src/debug/psynergy_test/scene_tables.o"(.text)
        "*/games/tla/src/debug/psynergy_test/exits.o"(.text)
        "*/games/tla/src/debug/psynergy_test/scene_events.o"(.text)
        "*/games/tla/src/debug/psynergy_test/s079c.o"(.text)
        "*/games/tla/src/debug/psynergy_test/prepare.o"(.text)
        "*/resource_6b8_overlay.o"(.text.x02008a10)
        "*/resource_6b8_overlay.o"(.text.x02008ad0)
        "*/resource_6b8_overlay.o"(.text.x02008c4c)
        "*/games/tla/src/debug/psynergy_test/glow_end.o"(.text)
        "*/games/tla/src/debug/psynergy_test/import.o"(.text)
        "*/games/tla/src/debug/psynergy_test/data.o"(.data)
    }
    .bss : { "*/resource_6b8_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
