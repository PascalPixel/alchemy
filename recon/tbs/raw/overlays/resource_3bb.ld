/* resource_3bb: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KABE/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KABE/TASK.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KABE/SPIN_ACTOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KABE/FLAG_TILES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KABE/PUSH_WALL.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KABE/ROLL_LOG.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KABE/SPECTATORS.o"(.text)
        "*/resource_3bb_overlay.o"(.text.x02008a1c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KABE/ENTRANCE_WALK.o"(.text)
        "*/resource_3bb_overlay.o"(.text.x02008bd4)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KABE/STAGE_INTRO.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KABE/STAGE_SCENES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KABE/LIFTED_ACTOR_COORDINATOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KABE/PUSH_BLOCK.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KABE/GUIDE_TALK.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KABE/PARTY_TILES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KOROSSEO/SOLO_COMPETITOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KABE/NEAREST.o"(.text)
        "*/resource_3bb_overlay.o"(.text.x02009df4)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KABE/PARTY_TALK.o"(.text)
        "*/resource_3bb_overlay.o"(.text.x02009ffc)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KABE/CHEER.o"(.text)
        "*/resource_3bb_overlay.o"(.text.x0200a3bc)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KOROSSEO/MODE_TASK.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KABE/TRANSITION.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KOROSSEO/FADE_IN_COMPETITOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KOROSSEO/RESTORE_COMPETITOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KABE/MOTION_RESET.o"(.text)
        "*/resource_3bb_overlay.o"(.text.x0200ae84)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KABE/MARKER.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KOROSSEO/ITEM_ICON.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KABE/HEX_TEXT.o"(.text)
        "*/resource_3bb_overlay.o"(.text.x0200b22c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KABE/APPROACH.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KOROSSEO/PATH_RIVAL.o"(.text)
        "*/resource_3bb_overlay.o"(.text.x0200b638)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KABE/DESCRIPTOR.o"(.text)
        "*/resource_3bb_overlay.o"(.text.x0200bae4)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KOROSSEO/SCENE_STATE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KABE/PUSH_AHEAD.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KABE/IMPORT.o"(.text)
        "*/resource_3bb_overlay.o"(.rodata)
    }
    .bss : { "*/resource_3bb_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
    /* The compiler calls its division helper by its ABI name; here it is the
       overlay's veneer into the resident IWRAM divider, as in MAIN.LD. */
    __divsi3 = Engine_MathDivide;
}
