/* resource_39b: its entry veneers, its listing with maintained code and
   the compiler library linked in place, as pret links objects and libgcc
   members in ROM order. Its import veneers follow its code. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_IRIGUCHI/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_IRIGUCHI/EFFECT_RECORD.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/OBJECT/STAGED_ACTOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_IRIGUCHI/LEADER_SPARKS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/MAKYURI/PALETTE_CYCLE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_IRIGUCHI/IRIGUCHI.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/MAKYURI/LIGHTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_IRIGUCHI/RISE_EFFECT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/MAKYURI/FOLLOW_LEADER.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_IRIGUCHI/IRIGUCHI2.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_IRIGUCHI/CLEAR_RECORD.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/MAKYURI/ACTOR_MOVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/MAKYURI/SPAWN_TIMER.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_IRIGUCHI/RELEASE_ATTACH.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_IRIGUCHI/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_39b_overlay.o"(.rodata.part1)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
    /* The compiler calls its division helper by its ABI name; here it is the
       overlay's veneer into the resident IWRAM divider, as in MAIN.LD. */
    __divsi3 = Engine_MathDivide;
}
