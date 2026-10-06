SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_IKE/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_IKE/GATE_PLACEMENT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_IKE/IMPORT.o"(.text)
        "*/resource_388_overlay.o"(.rodata.x02008108)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
