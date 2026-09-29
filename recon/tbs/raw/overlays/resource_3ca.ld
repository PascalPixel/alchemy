/* resource_3ca: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_FUNE/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_FUNE/TABLE_AUDIO.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_FUNE/GET_DEFAULT_RESULT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_FUNE/SCENE_CUES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_FUNE/PALETTE_CYCLE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_FUNE/DRIFTING_OBJECT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_FUNE/COUNTDOWN.o"(.text)
        "*/resource_3ca_overlay.o"(.text.x02008194)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_FUNE/STORE_FIELDS.o"(.text)
        "*/resource_3ca_overlay.o"(.text.x02008430)
        "*/resource_3ca_overlay.o"(.text.x02008b34)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_FUNE/SETUP_SCENE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_FUNE/HBLANK_SCROLL.o"(.text)
        "*/resource_3ca_overlay.o"(.text.x02008f80)
        "*/games/THE BROKEN SEAL/SRC/GRAPHICS/DISPLAY/SCROLL_INITIALIZE_HBLANK_DMA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_FUNE/STOP_DMA.o"(.text)
        "*/resource_3ca_overlay.o"(.text.x020091c4)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_FUNE/SCHEDULE_FADE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_FUNE/IMPORT.o"(.text)
        "*/resource_3ca_overlay.o"(.rodata.part1)
    }
    .bss : { "*/resource_3ca_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
