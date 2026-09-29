/* resource_394: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MAGARI/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MAGARI/GET_TABLE9170.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MAGARI/REDRAW_BOARD.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MAGARI/CALL_HELPER118C.o"(.text)
        "*/resource_394_overlay.o"(.text.x02008098)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MAGARI/PRESET_LAYOUT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MAGARI/REPAINT_BOARD.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MAGARI/UPDATE_SCROLL.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MAGARI/SCENE_SEQUENCE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MAGARI/RETURN_SEQUENCE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MAGARI/DEPART_SEQUENCE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MAGARI/BOARD_SETUP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MAGARI/PLACE_OBJECTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MAGARI/STAMP_RECORD.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MAGARI/FOUR_CELL_RUN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MAGARI/PUSH_BLOCK.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MAGARI/PALETTE_FADE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MAGARI/ADJUST_PALETTE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KORIMA/PALETTE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MAGARI/IMPORT.o"(.text)
        "*/resource_394_overlay.o"(.rodata)
    }
    .bss : { "*/resource_394_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
