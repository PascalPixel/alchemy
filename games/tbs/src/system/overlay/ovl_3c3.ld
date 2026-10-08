SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/suhara_gate/entry.o"(.text)
        "*/games/tbs/src/field/suhara_gate/gate.o"(.text)
        "*/games/tbs/src/field/suhara_gate/import.o"(.text)
        "*/games/tbs/src/field/suhara_gate/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
