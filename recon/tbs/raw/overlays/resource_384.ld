SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KUUPUAPPU_RUNPA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KUUPUAPPU_RUNPA/JAIL.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KUUPUAPPU_RUNPA/IMPORT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KUUPUAPPU_RUNPA/JAIL.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
