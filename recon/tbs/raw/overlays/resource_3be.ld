/* resource_3be: its entry veneers, its listing with maintained code and
   the compiler library linked in place, as pret links objects and libgcc
   members in ROM order. Its import veneers follow its code. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/KARAGORU_DOU/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KARAGORU_DOU/SET_EFFECT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/OBJECT/STAGED_ACTOR.o"(.text)
        "*/resource_3be_overlay.o"(.text.x02008cc0)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KARAGORU_DOU/GET_INITIAL.o"(.text)
        "*/resource_3be_overlay.o"(.text.x02008d20)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KARAGORU_DOU/RUN_ACTOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KARAGORU_DOU/STAGED_PAIR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KARAGORU_DOU/SPINNING_LEAP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KARAGORU_DOU/NOOP_SCENE.o"(.text)
        "*/resource_3be_overlay.o"(.text.x02009394)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KARAGORU_DOU/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_3be_overlay.o"(.rodata.part1)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
    /* The compiler calls its division helper by its ABI name; here it is the
       overlay's veneer into the resident IWRAM divider, as in MAIN.LD. */
    __divsi3 = IwramSignedDivideEntry;
}
