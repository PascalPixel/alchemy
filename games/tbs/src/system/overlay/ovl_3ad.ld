SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/runpa_dou/entry.o"(.text)
        "*/games/tbs/src/field/runpa_dou/dou.o"(.text)
        "*/games/tbs/src/field/runpa_dou/import.o"(.text)
        "*/games/tbs/src/field/runpa_dou/data.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
