SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_GATE/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_GATE/GATE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_GATE/IMPORT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_GATE/DATA.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
