SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/biribino_niwa/entry.o"(.text)
        "*/games/tbs/src/field/biribino_niwa/niwa.o"(.text)
        "*/games/tbs/src/field/biribino_niwa/import.o"(.text)
        "*/games/tbs/src/field/biribino_niwa/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
