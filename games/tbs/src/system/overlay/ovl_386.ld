SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/kuupuappu_heya_sai/entry.o"(.text)
        "*/games/tbs/src/field/kuupuappu_heya_sai/placement.o"(.text)
        "*/games/tbs/src/field/kuupuappu_heya_sai/import.o"(.text)
        "*/games/tbs/src/field/kuupuappu_heya_sai/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
