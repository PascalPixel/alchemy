SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/fune_hobashira/entry.o"(.text)
        "*/games/tbs/src/field/fune_hobashira/hobashira.o"(.text)
        "*/games/tbs/src/field/fune_hobashira/import.o"(.text)
        "*/games/tbs/src/field/fune_hobashira/data.o"(.data)
    }
    .bss ALIGN(16) (NOLOAD) :
    {
        "*/games/tbs/src/field/fune_hobashira/hobashira.o"(.bss)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
