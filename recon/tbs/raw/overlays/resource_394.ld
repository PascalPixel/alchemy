/* resource_394: its listing with maintained code
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_394_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MAGARI/GET_TABLE9170.o"(.text)
        "*/resource_394_overlay.o"(.text.x02008054)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MAGARI/CALL_HELPER118C.o"(.text)
        "*/resource_394_overlay.o"(.text.x02008098)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MAGARI/PRESET_LAYOUT.o"(.text)
        "*/resource_394_overlay.o"(.text.x02008194)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MAGARI/UPDATE_SCROLL.o"(.text)
        "*/resource_394_overlay.o"(.text.x020083f0)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MAGARI/PLACE_OBJECTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MAGARI/STAMP_RECORD.o"(.text)
        "*/resource_394_overlay.o"(.text.x02008be4)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MAGARI/ADJUST_PALETTE.o"(.text)
        "*/resource_394_overlay.o"(.text.x02008f34)
        "*/resource_394_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
