SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/haidia_arashi/entry.o"(.text)
        "*/games/tbs/src/field/haidia_arashi/arashi.o"(.text)
        "*/games/tbs/src/field/haidia_arashi/import.o"(.text)
        "*/games/tbs/src/field/haidia_arashi/data.o"(.data)
    }
    .bss (NOLOAD) :
    {
        "*/games/tbs/src/field/haidia_arashi/arashi.o"(.bss COMMON)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
