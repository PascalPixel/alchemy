SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/haidia_funka/entry.o"(.text)
        "*/games/tbs/src/field/haidia_funka/conditional_setup.o"(.text)
        "*/games/tbs/src/field/haidia_funka/import.o"(.text)
        "*/games/tbs/src/field/haidia_funka/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
