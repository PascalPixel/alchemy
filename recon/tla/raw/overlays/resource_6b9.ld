SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/debug/psynergy_test_2/entry.o"(.text)
        "*/games/tla/src/debug/psynergy_test_2/scene_tables.o"(.text)
        "*/games/tla/src/debug/psynergy_test_2/scene_events.o"(.text)
        "*/games/tla/src/debug/psynergy_test_2/s0104.o"(.text)
        "*/games/tla/src/debug/psynergy_test_2/prepare.o"(.text)
        "*/games/tla/src/debug/psynergy_test_2/s015c.o"(.text)
        "*/recon/tla/raw/src/debug/psynergy_test_2/s01e0.o"(.text)
        "*/games/tla/src/debug/psynergy_test_2/s029c.o"(.text)
        "*/games/tla/src/debug/psynergy_test_2/sparks.o"(.text)
        "*/games/tla/src/debug/psynergy_test_2/scale.o"(.text)
        "*/games/tla/src/debug/psynergy_test_2/drop.o"(.text)
        "*/games/tla/src/debug/psynergy_test_2/s0468.o"(.text)
        "*/games/tla/src/debug/psynergy_test_2/import.o"(.text)
        "*/games/tla/src/debug/psynergy_test_2/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
