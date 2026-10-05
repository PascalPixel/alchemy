SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/RUNPA_SUHARA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/RUNPA_SUHARA/SUHARA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/RUNPA_SUHARA/IMPORT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/RUNPA_SUHARA/SUHARA.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
