SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/biribino_dou/entry.o"(.text)
        "*/games/tbs/src/field/biribino_dou/dou.o"(.text)
        "*/games/tbs/src/field/biribino_dou/import.o"(.text)
        "*/games/tbs/src/field/biribino_dou/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
