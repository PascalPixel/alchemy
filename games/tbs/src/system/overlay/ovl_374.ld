SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/haidia_ie/entry.o"(.text)
        "*/games/tbs/src/field/haidia_ie/ie.o"(.text)
        "*/games/tbs/src/field/haidia_ie/import.o"(.text)
        "*/games/tbs/src/field/haidia_ie/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
