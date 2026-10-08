SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/suhara_heya/entry.o"(.text)
        "*/games/tbs/src/field/suhara_heya/heya.o"(.text)
        "*/games/tbs/src/field/suhara_heya/import.o"(.text)
        "*/games/tbs/src/field/suhara_heya/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
