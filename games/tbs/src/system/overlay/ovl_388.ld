SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/goma_ike/entry.o"(.text)
        "*/games/tbs/src/field/goma_ike/gate_placement.o"(.text)
        "*/games/tbs/src/field/goma_ike/import.o"(.text)
        "*/games/tbs/src/field/goma_ike/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
