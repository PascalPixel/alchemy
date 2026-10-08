SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_NIWA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_NIWA/NIWA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_NIWA/IMPORT.o"(.text)
        "*/resource_38e_overlay.o"(.rodata.x02008bd4)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
