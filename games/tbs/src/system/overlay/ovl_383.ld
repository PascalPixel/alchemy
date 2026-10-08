SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/kuupuappu_heya/entry.o"(.text)
        "*/games/tbs/src/field/kuupuappu_heya/heya.o"(.text)
        "*/games/tbs/src/field/kuupuappu_heya/import.o"(.text)
        "*/games/tbs/src/field/kuupuappu_heya/data.o"(.data)
    }
    .bss : { "*/resource_383_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
