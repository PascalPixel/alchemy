SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/IMIRU_FUCHIN/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/IMIRU_FUCHIN/FUCHIN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/IMIRU_FUCHIN/IMPORT.o"(.text)
        "*/resource_39a_overlay.o"(.rodata.x0200a418)
    }
    .bss (NOLOAD) :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/IMIRU_FUCHIN/FUCHIN.o"(.bss COMMON)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
