SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/soru_roka/entry.o"(.text)
        "*/games/tbs/src/field/soru_roka/scene_37e.o"(.text)
        "*/games/tbs/src/field/soru_roka/import.o"(.text)
        "*/games/tbs/src/field/soru_roka/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
