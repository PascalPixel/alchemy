SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/haidia_heya/entry.o"(.text)
        "*/games/tbs/src/field/haidia_heya/heya.o"(.text)
        "*/games/tbs/src/field/haidia_heya/import.o"(.text)
        "*/games/tbs/src/field/haidia_heya/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
