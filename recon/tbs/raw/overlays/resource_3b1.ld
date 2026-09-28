/* resource_3b1: its listing with maintained code linked in place,
   as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_3b1_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/WALKER_STEP.o"(.text)
        "*/resource_3b1_overlay.o"(.text.x02008284)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/STEP_SCRIPT.o"(.text)
        "*/resource_3b1_overlay.o"(.text.x0200854c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/TURN_ACTOR.o"(.text)
        "*/resource_3b1_overlay.o"(.text.x02009280)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/FLAG_BRANCH_SEQUENCE.o"(.text)
        "*/resource_3b1_overlay.o"(.text.x02009a08)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/ENTRY_STATE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/FLAG_LAYOUT.o"(.text)
        "*/resource_3b1_overlay.o"(.text.x0200b51c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/PLACE_FOUND_ACTORS.o"(.text)
        "*/resource_3b1_overlay.o"(.text.x0200b8ac)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/COND_ACT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/PLACE_ANCHOR.o"(.text)
        "*/resource_3b1_overlay.o"(.text.x0200c670)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/POSE_ACTOR.o"(.text)
        "*/resource_3b1_overlay.o"(.text.x0200c86c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/FIRST_SET_FLAG.o"(.text)
        "*/resource_3b1_overlay.o"(.text.x0200d004)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/CUE_TIMER.o"(.text)
        "*/resource_3b1_overlay.o"(.text.x0200dca4)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/PROGRESS_SHOW.o"(.text)
        "*/resource_3b1_overlay.o"(.text.x0200e110)
        "*/resource_3b1_overlay.o"(.rodata.part1)
    }
    .bss : { "*/resource_3b1_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
