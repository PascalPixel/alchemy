SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/toreto_eda/entry.o"(.text)
        "*/games/tbs/src/field/toreto_eda/eda.o"(.text)
        "*/games/tbs/src/field/toreto_eda/import.o"(.text)
        "*/games/tbs/src/field/toreto_eda/data.o"(.data)
    }
    .bss ALIGN(16) (NOLOAD) :
    {
        "*/games/tbs/src/field/toreto_eda/eda.o"(.bss)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
