SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_IKE/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_IKE/GATE_PLACEMENT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_IKE/IMPORT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_IKE/DATA.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
