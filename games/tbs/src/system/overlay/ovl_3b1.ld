SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/fune_heya/entry.o"(.text)
        "*/games/tbs/src/field/fune_heya/heya.o"(.text)
        "*/games/tbs/src/field/fune_heya/import.o"(.text)
        "*/games/tbs/src/field/fune_heya/data.o"(.data)
    }
    .bss : { "*/resource_3b1_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
