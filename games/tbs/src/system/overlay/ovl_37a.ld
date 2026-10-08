SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/soru_nichigetsu/entry.o"(.text)
        "*/games/tbs/src/field/soru_nichigetsu/nichigetsu.o"(.text)
        "*/games/tbs/src/field/soru_nichigetsu/import.o"(.text)
        "*/games/tbs/src/field/soru_nichigetsu/data.o"(.data)
    }
    .bss : { "*/resource_37a_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
