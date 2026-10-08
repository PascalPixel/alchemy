SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/IMIRU_MURA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/IMIRU_MURA/MURA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/IMIRU_MURA/CHOREOGRAPHY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/IMIRU_MURA/IMPORT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/IMIRU_MURA/DATA.o"(.data)
    }
    .bss : { "*/resource_399_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
