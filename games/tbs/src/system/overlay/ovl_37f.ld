SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/soru_iriguchi/entry.o"(.text)
        "*/games/tbs/src/field/soru_iriguchi/iriguchi.o"(.text)
        "*/games/tbs/src/field/soru_iriguchi/import.o"(.text)
        "*/games/tbs/src/field/soru_iriguchi/data.o"(.data)
    }
    .bss (NOLOAD) :
    {
        "*/games/tbs/src/field/soru_iriguchi/iriguchi.o"(.bss)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
