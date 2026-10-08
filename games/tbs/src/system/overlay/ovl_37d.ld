SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/soru_meiro/entry.o"(.text)
        "*/games/tbs/src/field/soru_meiro/scene_37d.o"(.text)
        "*/games/tbs/src/field/soru_meiro/import.o"(.text)
        "*/games/tbs/src/field/soru_meiro/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
