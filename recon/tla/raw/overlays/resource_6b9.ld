SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/debug/psynergy_test_2/entry.o"(.text)
        "*/games/tla/src/debug/psynergy_test_2/scene_tables.o"(.text)
        "*/games/tla/src/debug/psynergy_test_2/scene_events.o"(.text)
        "*/resource_6b9_overlay.o"(.text.x02008104)
        "*/games/tla/src/debug/psynergy_test_2/prepare.o"(.text)
        "*/resource_6b9_overlay.o"(.text.x0200815c)
        "*/resource_6b9_overlay.o"(.text.x020081e0)
        "*/resource_6b9_overlay.o"(.text.x0200829c)
        "*/games/tla/src/debug/psynergy_test_2/sparks.o"(.text)
        "*/games/tla/src/debug/psynergy_test_2/scale.o"(.text)
        "*/games/tla/src/debug/psynergy_test_2/drop.o"(.text)
        "*/resource_6b9_overlay.o"(.text.x02008468)
        "*/games/tla/src/debug/psynergy_test_2/import.o"(.text)
        "*/games/tla/src/debug/psynergy_test_2/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
