SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/common/runpa_suhara/entry.o"(.text)
        "*/games/tbs/src/field/common/runpa_suhara/suhara.o"(.text)
        "*/games/tbs/src/field/common/runpa_suhara/import.o"(.text)
        "*/games/tbs/src/field/common/runpa_suhara/suhara.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
