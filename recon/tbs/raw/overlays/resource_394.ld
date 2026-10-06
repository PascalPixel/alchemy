SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MAGARI/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MAGARI/MAGARI.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/MAP/PANEL.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MAGARI/PUZZLE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KORIMA/PALETTE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MAGARI/IMPORT.o"(.text)
        "*/resource_394_overlay.o"(.rodata.x0200911c)
    }
    .bss : { "*/resource_394_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
