SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/fune_kanpan/entry.o"(.text)
        "*/games/tbs/src/field/fune_kanpan/update_scale.o"(.text)
        "*/games/tbs/src/field/fune_kanpan/kanpan.o"(.text)
        "*/games/tbs/src/field/fune_kanpan/actors.o"(.text)
        "*/games/tbs/src/field/fune_kanpan/arrival.o"(.text)
        "*/games/tbs/src/field/fune_kanpan/import.o"(.text)
        "*/games/tbs/src/field/fune_kanpan/data.o"(.data)
    }
    .bss ALIGN(16) (NOLOAD) :
    {
        "*/games/tbs/src/field/fune_kanpan/kanpan.o"(.data .bss COMMON)
        "*/games/tbs/src/field/fune_kanpan/arrival.o"(COMMON)
        "*/games/tbs/src/field/fune_kanpan/actors.o"(COMMON)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
