/* resource_3b1: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/STAGED.o"(.text)
        "*/resource_3b1_overlay.o"(.text.x0200812c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/EFFECT9.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/STEP_SCRIPT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/DIALOG_A.o"(.text)
        "*/resource_3b1_overlay.o"(.text.x02008a80)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/DIALOG_A2.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/TURN_ACTOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/DIALOG_B.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/FLAG_BRANCH_SEQUENCE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/EXTENDED_CHOREOGRAPHY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/ENTRY_STATE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/FLAG_LAYOUT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/CASCADE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/PLACE_FOUND_ACTORS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/PRESENT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/MONSTERS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/PRESENT2.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/COND_ACT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/PLACE_ANCHOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/INSTALL.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/POSE_ACTOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/STEPS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/FIRST_SET_FLAG.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/FORMATN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/BELOWDECKS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/FORMATN2.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/CUE_TIMER.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/MAPEVENT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/PROGRESS_SHOW.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/SCAN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/IMPORT.o"(.text)
        "*/resource_3b1_overlay.o"(.rodata.part1)
    }
    .bss : { "*/resource_3b1_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
