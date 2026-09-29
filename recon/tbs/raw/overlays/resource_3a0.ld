/* resource_3a0: its entry veneers, its listing with maintained code and
   the compiler library linked in place, as pret links objects and libgcc
   members in ROM order. Its import veneers follow its code. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/SHIAN_MURA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SHIAN_MURA/EFFECT_SETUP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SHIAN_MURA/EFFECT_CONFIG.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SHIAN_MURA/FIXED_EFFECT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SHIAN_MURA/PRESENTATION_FLAG.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SHIAN_MURA/ROTATING.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SHIAN_MURA/ACTOR_TRACKING.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SHIAN_MURA/GATE_WATCH.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SHIAN_MURA/PRIMARY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SHIAN_MURA/NPC_MEET.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SHIAN_MURA/SCENE_MESSAGE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SHIAN_MURA/LEADER_HOP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SHIAN_MURA/DISPATCH.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SHIAN_MURA/ACTOR19_SCRIPT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SHIAN_MURA/TERTIARY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SHIAN_MURA/SETUP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SHIAN_MURA/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_3a0_overlay.o"(.rodata.part1)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
    /* The compiler calls its division helper by its ABI name; here it is the
       overlay's veneer into the resident IWRAM divider, as in MAIN.LD. */
    __divsi3 = IwramSignedDivideEntry;
}
