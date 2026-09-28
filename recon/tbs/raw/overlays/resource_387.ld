/* resource_387: its listing with maintained code linked in place,
   as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_387_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/resource_387_overlay.o"(.text.x0200813c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_IRIGUCHI/ENTRY_STATE.o"(.text)
        "*/resource_387_overlay.o"(.text.x0200867c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_IRIGUCHI/SET_ENTRANCE_FLAG.o"(.text)
        "*/resource_387_overlay.o"(.text.x02008da8)
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_IRIGUCHI/SPINNING_LEAP.o"(.text)
        "*/resource_387_overlay.o"(.text.x02008f8c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_IRIGUCHI/GIVE_SHAMANS_ROD.o"(.text)
        "*/resource_387_overlay.o"(.text.x02009084)
        "*/resource_387_overlay.o"(.rodata.part1)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
