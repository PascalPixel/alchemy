SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_FUNE/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_FUNE/FUNE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_FUNE/SCENE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_FUNE/WAVES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/GRAPHICS/DISPLAY/SCROLL_INITIALIZE_HBLANK_DMA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_FUNE/STOP_DMA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_FUNE/IMPORT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_FUNE/DATA.o"(.data)
    }
    .bss : { "*/resource_3ca_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
