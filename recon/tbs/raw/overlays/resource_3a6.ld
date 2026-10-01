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
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_DOU/DOU.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_DOU/PROBED_COLUMN_SCENE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_DOU/ACTOR_NINE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_DOU/LOWERED_ACTOR_SCENE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_DOU/DOU2.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_DOU/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_3a6_overlay.o"(.rodata.x02009da0)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
