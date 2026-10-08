SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/torebi_kyuden/entry.o"(.text)
        "*/games/tbs/src/field/torebi_kyuden/kyuden.o"(.text)
        "*/games/tbs/src/field/torebi_kyuden/import.o"(.text)
        "*/games/tbs/src/field/torebi_kyuden/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
