/* resource_3c9: its entry veneers, its listing with maintained code and
   the compiler library linked in place, as pret links objects and libgcc
   members in ROM order. Its import veneers follow its code. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_CHOJO/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_CHOJO/OBJECTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_CHOJO/SETUP.o"(.text)
        "*/resource_3c9_overlay.o"(.text.x020088b4)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_CHOJO/SCENE_A.o"(.text)
        "*/resource_3c9_overlay.o"(.text.x020092c8)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_CHOJO/EFFECTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_CHOJO/PAIR_EFFECT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_CHOJO/STEPS.o"(.text)
        "*/resource_3c9_overlay.o"(.text.x0200a360)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_CHOJO/ORBIT.o"(.text)
        "*/resource_3c9_overlay.o"(.text.x0200b6d0)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_CHOJO/BEAM_ACTORS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_CHOJO/SCRIPTED_PRESENTATION.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_CHOJO/ACTOR_TRANSITION.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_CHOJO/RESTAGE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_CHOJO/EXTENDED_ACTOR_TRANSITION.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_CHOJO/PARK.o"(.text)
        "*/resource_3c9_overlay.o"(.text.x0200d6a0)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_CHOJO/RISE.o"(.text)
        "*/resource_3c9_overlay.o"(.text.x0200da26)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_CHOJO/FINALE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_CHOJO/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_3c9_overlay.o"(.rodata.part1)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
    /* The compiler calls its division helper by its ABI name; here it is the
       overlay's veneer into the resident IWRAM divider, as in MAIN.LD. */
    __divsi3 = IwramSignedDivideEntry;
}
