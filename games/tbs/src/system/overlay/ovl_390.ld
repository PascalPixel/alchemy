SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/korima_heya/entry.o"(.text)
        "*/games/tbs/src/field/korima_heya/entry_layout.o"(.text)
        "*/games/tbs/src/field/korima_heya/import.o"(.text)
        "*/games/tbs/src/field/korima_heya/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
