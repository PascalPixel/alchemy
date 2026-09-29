/* resource_3c4: its entry veneers, its listing with maintained code and
   the compiler library linked in place, as pret links objects and libgcc
   members in ROM order. Its import veneers follow its code. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_CHIKA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/OBJECT/STAGED_ACTOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_CHIKA/OBJECTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_CHIKA/ACTORS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_CHIKA/TRACKED_ACTOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_CHIKA/SLOTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_CHIKA/FLICKER_EFFECT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_CHIKA/WAIT.o"(.text)
        "*/resource_3c4_overlay.o"(.text.x020090c2)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_CHIKA/RADIAL.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_CHIKA/SCENE_TABLES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_CHIKA/TABLES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_CHIKA/RECT_STEP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_CHIKA/POINT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_CHIKA/LAYOUT_STEP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_CHIKA/STEPS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_CHIKA/TWO_RECTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_CHIKA/MAP_INIT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_CHIKA/SETTLE_STEPS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_CHIKA/SEQUENCE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_CHIKA/MARK_ACTOR_CELLS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_CHIKA/ENTRY_STATE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_CHIKA/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_3c4_overlay.o"(.rodata.part1)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
    /* The compiler calls its division helper by its ABI name; here it is the
       overlay's veneer into the resident IWRAM divider, as in MAIN.LD. */
    __divsi3 = Engine_MathDivide;
}
