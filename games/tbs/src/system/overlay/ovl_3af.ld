SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/fune_kanpan/entry.o"(.text)
        "*/games/tbs/src/field/fune_kanpan/kanpan.o"(.text)
        "*/games/tbs/src/field/fune_kanpan/import.o"(.text)
        "*/games/tbs/src/field/fune_kanpan/data.o"(.data)
    }
    .bss ALIGN(16) (NOLOAD) :
    {
        "*/games/tbs/src/field/fune_kanpan/kanpan.o"(.data .bss COMMON)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
