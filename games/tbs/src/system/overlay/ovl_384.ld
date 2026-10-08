SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/common/kuupuappu_runpa/entry.o"(.text)
        "*/games/tbs/src/field/common/kuupuappu_runpa/jail.o"(.text)
        "*/games/tbs/src/field/common/kuupuappu_runpa/import.o"(.text)
        "*/games/tbs/src/field/common/kuupuappu_runpa/jail.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
