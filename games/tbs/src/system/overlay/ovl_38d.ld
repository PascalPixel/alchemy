SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/biribino_kyuden/entry.o"(.text)
        "*/games/tbs/src/field/biribino_kyuden/kyuden.o"(.text)
        "*/games/tbs/src/field/biribino_kyuden/import.o"(.text)
        "*/games/tbs/src/field/biribino_kyuden/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
