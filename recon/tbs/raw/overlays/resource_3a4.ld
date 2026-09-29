/* resource_3a4: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/FACING.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/GET_TABLES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/SCENE_DATA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/GET_PLACEMENTS.o"(.text)
        "*/resource_3a4_overlay.o"(.text.x020082cc)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/SET_PIECE.o"(.text)
        "*/resource_3a4_overlay.o"(.text.x02008d2c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/SHARED.o"(.text)
        "*/resource_3a4_overlay.o"(.text.x02009398)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/GET_EVENTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/TIMED_ACTOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/MULTI_PHASE.o"(.text)
        "*/resource_3a4_overlay.o"(.text.x02009d0c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/AREA_SCENE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/ENTRY_STATE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/SCENE_SETUP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/FLAGGED_ACTORS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/SCENE_SETUP2.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/DRIFTING_LEAF.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/SEQUENCE.o"(.text)
        "*/resource_3a4_overlay.o"(.text.x0200af10)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/EVENT_SCRIPT.o"(.text)
        "*/resource_3a4_overlay.o"(.text.x0200b410)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/MARKER.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/SETTLE_MOUNT.o"(.text)
        "*/resource_3a4_overlay.o"(.text.x0200b5a8)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/CUE288.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/ROLL_STEP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/ROLL_OBJECT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/PALETTE_STEP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/PALETTE_START.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/IMPORT.o"(.text)
        "*/resource_3a4_overlay.o"(.rodata)
    }
    .bss : { "*/resource_3a4_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
