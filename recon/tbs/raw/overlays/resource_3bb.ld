/* resource_3bb: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KABE/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KABE/KABE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KOROSSEO/SOLO_COMPETITOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KABE/NEAREST.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KOROSSEO/SOLO_ROUND.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KABE/KABE3.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KOROSSEO/KOROSSEO.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KABE/TRANSITION.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KOROSSEO/KOROSSEO2.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KABE/MOTION_RESET.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KOROSSEO/MARKER.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KABE/MARKER.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KOROSSEO/ITEM_ICON.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KABE/KABE4.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KOROSSEO/PATH_RIVAL.o"(.text)
        "*/resource_3bb_overlay.o"(.text.x0200b638)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KABE/KABE5.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KOROSSEO/SCENE_STATE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KABE/PUSH_AHEAD.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KABE/IMPORT.o"(.text)
        "*/resource_3bb_overlay.o"(.rodata.x0200c0c0)
    }
    .bss : { "*/resource_3bb_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
    /* The compiler calls its division helper by its ABI name; here it is the
       overlay's veneer into the resident IWRAM divider, as in MAIN.LD. */
    __divsi3 = Engine_MathDivide;
}
