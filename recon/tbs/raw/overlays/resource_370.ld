/* resource_370: its listing with maintained code
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_370_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/CLEAR/BLEND_FADE.o"(.text)
        "*/resource_370_overlay.o"(.text.x020081fc)
        "*/games/THE BROKEN SEAL/SRC/MENU/CLEAR/NAME_CHARACTER.o"(.text)
        "*/resource_370_overlay.o"(.text.x02008384)
        "*/games/THE BROKEN SEAL/SRC/MENU/CLEAR/TILE_GRID.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/CLEAR/FILL_TILE_BLOCK.o"(.text)
        "*/resource_370_overlay.o"(.text.x02008de4)
        "*/games/THE BROKEN SEAL/SRC/MENU/CLEAR/ENCODE_PASSWORD.o"(.text)
        "*/resource_370_overlay.o"(.text.x020092c8)
        "*/resource_370_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
