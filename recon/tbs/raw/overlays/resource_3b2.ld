/* resource_3b2: its entry veneers, its listing with maintained code and
   the compiler library linked in place, as pret links objects and libgcc
   members in ROM order. Its import veneers follow its code. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/OBJECT/STAGED_ACTOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/EFFOWNER.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/PROLOGUE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/GET_ENTRANCES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/PROLOGUE2.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/GET_PLACEMENTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/PROLOGUE3.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/GET_EVENTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/PROLOGUE4.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/SCENE_INIT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/PROLOGUE5.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/ESCAPE_COLUMN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/SCRIPT37.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/AREA_ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/ISLAND_AND_CAVES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/SETTLE_BLOCK_NORTH.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/ESCAPE18.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/SETTLE_BLOCK_NORTH_B.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/ESCAPE9.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/SETTLE_BLOCK_WEST.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/SETTLE_BLOCK.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/ESCAPE_B.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_3b2_overlay.o"(.rodata.part1)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
    /* The compiler calls its division helper by its ABI name; here it is the
       overlay's veneer into the resident IWRAM divider, as in MAIN.LD. */
    __divsi3 = Engine_MathDivide;
}
