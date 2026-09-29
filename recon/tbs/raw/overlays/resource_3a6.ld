/* resource_3a6: its entry veneers, its listing with maintained code and
   the compiler library linked in place, as pret links objects and libgcc
   members in ROM order. Its import veneers follow its code. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_DOU/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_DOU/OBJECTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/OBJECT/STAGED_ACTOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_DOU/ACTOR_STEPS.o"(.text)
        "*/resource_3a6_overlay.o"(.text.x02008d20)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_DOU/SCENE_EXITS.o"(.text)
        "*/resource_3a6_overlay.o"(.text.x02008d80)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_DOU/REGION_ROWS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_DOU/SINK_PILLAR_23.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_DOU/SINK_PILLAR_27.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_DOU/SEQUENCES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_DOU/PROBED_COLUMN_SCENE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_DOU/ACTOR_NINE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_DOU/LOWERED_ACTOR_SCENE.o"(.text)
        "*/resource_3a6_overlay.o"(.text.x0200969c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_DOU/FLAG301.o"(.text)
        "*/resource_3a6_overlay.o"(.text.x02009984)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_DOU/ACTOR_ELEVEN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_DOU/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_3a6_overlay.o"(.rodata.part1)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
    /* The compiler calls its division helper by its ABI name; here it is the
       overlay's veneer into the resident IWRAM divider, as in MAIN.LD. */
    __divsi3 = IwramSignedDivideEntry;
}
