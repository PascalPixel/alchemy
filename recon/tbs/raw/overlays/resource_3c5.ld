/* resource_3c5: its entry veneers, its listing with maintained code and
   the compiler library linked in place, as pret links objects and libgcc
   members in ROM order. Its import veneers follow its code. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_IRIGUCHI/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/OBJECT/STAGED_ACTOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_IRIGUCHI/OBJECTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_IRIGUCHI/MOTION.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_IRIGUCHI/LANDING.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_IRIGUCHI/GET_ENTRANCES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_IRIGUCHI/EXITS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_IRIGUCHI/GET_PLACEMENTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_IRIGUCHI/JUMP_FROM_LEDGE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_IRIGUCHI/BRANCH.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_IRIGUCHI/DOOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_IRIGUCHI/FIND_FWD.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_IRIGUCHI/PLACING.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_IRIGUCHI/FACING.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_IRIGUCHI/GET_EVENTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_IRIGUCHI/REGION.o"(.text)
        "*/resource_3c5_overlay.o"(.text.x0200a8a0)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BABI_IRIGUCHI/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_3c5_overlay.o"(.rodata.part1)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
    /* The compiler calls its division helper by its ABI name; here it is the
       overlay's veneer into the resident IWRAM divider, as in MAIN.LD. */
    __divsi3 = IwramSignedDivideEntry;
}
