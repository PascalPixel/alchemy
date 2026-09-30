/* resource_3bc: its entry veneers, its listing with maintained code and
   the compiler library linked in place, as pret links objects and libgcc
   members in ROM order. Its import veneers follow its code. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_MARUTA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_MARUTA/HOOKS.o"(.text)
        "*/resource_3bc_overlay.o"(.text.x0200804c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_MARUTA/LOG_ROLLING.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_MARUTA/ACTIVE_ACTOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_MARUTA/COMPETITOR_TALK.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_MARUTA/STAGE_START.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_MARUTA/EVENT_STATE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_MARUTA/STAGE_SETUP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_MARUTA/PARTICLES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_MARUTA/GREET_SCENE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_MARUTA/ARRIVAL.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_MARUTA/SECOND_ARRIVAL.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_MARUTA/STAGE_INTRO.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_MARUTA/ACTOR_MOTION.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KOROSSEO/SOLO_COMPETITOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_MARUTA/OBSTACLE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KOROSSEO/SOLO_ROUND.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_MARUTA/MARUTA2.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KOROSSEO/KOROSSEO.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_MARUTA/TRANSITION.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KOROSSEO/FADE_IN_COMPETITOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KOROSSEO/RESTORE_COMPETITOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_MARUTA/MOTION_RESET.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KOROSSEO/MARKER.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_MARUTA/PALETTE_TASK.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KOROSSEO/ITEM_ICON.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_MARUTA/MARUTA3.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KOROSSEO/PATH_RIVAL.o"(.text)
        "*/resource_3bc_overlay.o"(.text.x0200c0d0)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_MARUTA/MARUTA4.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KOROSSEO/SCENE_STATE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_MARUTA/PUSH.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_MARUTA/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_3bc_overlay.o"(.rodata.part1)
    }
    .bss (NOLOAD) : { "*/resource_3bc_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
    /* The compiler calls its division helper by its ABI name; here it is the
       overlay's veneer into the resident IWRAM divider, as in MAIN.LD. */
    __divsi3 = Engine_MathDivide;
}
