/* resource_3bf: its entry veneers, its listing with maintained code and
   the compiler library linked in place, as pret links objects and libgcc
   members in ROM order. Its import veneers follow its code, then the
   soft-float members its double arithmetic pulls in; their NaN constant
   sits in the rodata where the ROM holds it. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/OBJECT/STAGED_ACTOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/GET_ENTRANCES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/EMPTY_DATA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/GET_EXITS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/GET_PLACEMENTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/GET_EVENTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/BRIDGE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/SET_POSITION_PAIRS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/CELLS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/GUARDS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/FORTRESS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/CAUGHT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/TURN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/GUARD_LINES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/WHO_SCENE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/SEQUENCE_TAIL.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/CHALLENGE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/INSPECT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/STORY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/RESCUE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/ACTOR21.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/VARIANTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/ACTOR_LINES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/ACTOR20.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/ACTOR13.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/SCENE_INIT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/SETUP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        *libgcc.a:_addsub_df.o(.text)
        *libgcc.a:_si_to_df.o(.text)
        *libgcc.a:_df_to_si.o(.text)
        *libgcc.a:_lshrdi3.o(.text)
        *libgcc.a:_pack_df.o(.text)
        *libgcc.a:_unpack_df.o(.text)
        "*/resource_3bf_overlay.o"(.rodata.part1)
        *libgcc.a:_thenan_df.o(.rodata)
        "*/resource_3bf_overlay.o"(.rodata.part2)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
