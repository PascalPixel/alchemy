SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/babi_fune/entry.o"(.text)
        "*/games/tbs/src/field/babi_fune/fune.o"(.text)
        "*/games/tbs/src/field/babi_fune/scene.o"(.text)
        "*/games/tbs/src/field/babi_fune/waves.o"(.text)
        "*/games/tbs/src/graphics/display/scroll_initialize_hblank_dma.o"(.text)
        "*/games/tbs/src/field/babi_fune/stop_dma.o"(.text)
        "*/games/tbs/src/field/babi_fune/import.o"(.text)
        "*/games/tbs/src/field/babi_fune/data.o"(.data)
    }
    .bss : { "*/resource_3ca_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
