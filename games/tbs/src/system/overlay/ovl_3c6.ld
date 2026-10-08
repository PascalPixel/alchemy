SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/raribero_machi/entry.o"(.text)
        "*/games/tbs/src/field/raribero_machi/machi.o"(.text)
        "*/games/tbs/src/field/raribero_machi/import.o"(.text)
        "*/games/tbs/src/field/raribero_machi/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
