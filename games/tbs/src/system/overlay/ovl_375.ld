SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/haidia_sukureta/entry.o"(.text)
        "*/games/tbs/src/field/haidia_sukureta/sukureta.o"(.text)
        "*/games/tbs/src/field/haidia_sukureta/import.o"(.text)
        "*/games/tbs/src/field/haidia_sukureta/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
