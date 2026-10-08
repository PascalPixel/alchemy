SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/karei_kyuden/entry.o"(.text)
        "*/games/tbs/src/field/karei_kyuden/kyuden.o"(.text)
        "*/games/tbs/src/field/karei_kyuden/import.o"(.text)
        "*/games/tbs/src/field/karei_kyuden/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
