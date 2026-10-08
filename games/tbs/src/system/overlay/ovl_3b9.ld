SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/korashiamu_iriguchi/entry.o"(.text)
        "*/games/tbs/src/field/korashiamu_iriguchi/iriguchi.o"(.text)
        "*/games/tbs/src/field/korashiamu_iriguchi/import.o"(.text)
        "*/games/tbs/src/field/korashiamu_iriguchi/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
