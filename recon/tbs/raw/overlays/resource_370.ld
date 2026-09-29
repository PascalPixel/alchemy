/* resource_370: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/MENU/CLEAR/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/CLEAR/GET_SCRIPT.o"(.text)
        "*/resource_370_overlay.o"(.text.x02008054)
        "*/games/THE BROKEN SEAL/SRC/MENU/CLEAR/BLEND_FADE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/CLEAR/BUTTON_CODES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/CLEAR/NAME_CHARACTER.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/CLEAR/MODE_MASK.o"(.text)
        "*/resource_370_overlay.o"(.text.x020083cc)
        "*/games/THE BROKEN SEAL/SRC/MENU/CLEAR/TILE_GRID.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/CLEAR/FILL_TILE_BLOCK.o"(.text)
        "*/resource_370_overlay.o"(.text.x02008de4)
        "*/games/THE BROKEN SEAL/SRC/MENU/CLEAR/ENCODE_PASSWORD.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/CLEAR/SAVE_MENU.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/CLEAR/IMPORT.o"(.text)
        "*/resource_370_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
