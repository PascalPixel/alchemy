SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/soru_dou/entry.o"(.text)
        "*/games/tbs/src/field/soru_dou/table_update.o"(.text)
        "*/games/tbs/src/field/soru_dou/import.o"(.text)
        "*/games/tbs/src/field/soru_dou/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
