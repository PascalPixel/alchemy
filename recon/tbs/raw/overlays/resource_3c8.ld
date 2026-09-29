/* resource_3c8: its entry veneers, its listing with maintained code and
   the compiler library linked in place, as pret links objects and libgcc
   members in ROM order. Its import veneers follow its code. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/OBJECTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/ENTRY_SETUP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/GET_ENTRANCES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/SCENE_DATA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/GET_PLACEMENTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/STATUE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/FLOOR_SWITCH.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/CAMERA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/ACTOR_LEAP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/SLOTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/BRIDGE_SHIFT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/FLAG_301_CELL_PUSH.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/RECT_STEP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/SETTLE_BLOCKS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/HEIGHT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/LOWER_BLOCKS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/PICK_SLOT.o"(.text)
        "*/resource_3c8_overlay.o"(.text.x0200a47c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/THREE_CALL.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/BRIDGE_RETRACT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/BRIDGE_EXTEND.o"(.text)
        "*/resource_3c8_overlay.o"(.text.x0200ab14)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/TILES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/GET_EVENTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/STEP_SLOTS.o"(.text)
        "*/resource_3c8_overlay.o"(.text.x0200b068)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/PASS_RANGE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/DUST_STEP.o"(.text)
        "*/resource_3c8_overlay.o"(.text.x0200c2bc)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/CALL_PAIR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/PARTICLE_WAVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/EAST_PARTICLE_WAVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/RISING_SPRAY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/LEADER_DROP.o"(.text)
        "*/resource_3c8_overlay.o"(.text.x0200cbd8)
        "*/games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_3c8_overlay.o"(.rodata.part1)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
    /* The compiler calls its division helper by its ABI name; here it is the
       overlay's veneer into the resident IWRAM divider, as in MAIN.LD. */
    __divsi3 = IwramSignedDivideEntry;
}
