SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_HEYA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_HEYA/MESSAGE_BRANCH.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_HEYA/IMPORT.o"(.text)
        "*/resource_38c_overlay.o"(.rodata.x02008598)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
