/* resource_378: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHINDEN_HEYA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHINDEN_HEYA/UPDATE_FACING.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHINDEN_HEYA/ENTRY_STATE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHINDEN_HEYA/RUN_ACTOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHINDEN_HEYA/ALTAR_SCENE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHINDEN_HEYA/LEADER_PRIORITY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHINDEN_HEYA/FACING_GATED.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHINDEN_HEYA/COPY_ACTOR_POSE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHINDEN_HEYA/RESET_OBJECT14.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHINDEN_HEYA/FOLLOW_LEADER.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHINDEN_HEYA/STEP_ELLIPSE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHINDEN_HEYA/SPAWN_EFFECT.o"(.text)
        "*/resource_378_overlay.o"(.text.x0200a7d4)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHINDEN_HEYA/RUN_PAIRED.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHINDEN_HEYA/ITEM_ICON.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHINDEN_HEYA/RISE_SPARK.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHINDEN_HEYA/SPAWN_SPARK.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHINDEN_HEYA/APPLY_TWO.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHINDEN_HEYA/IMPORT.o"(.text)
        "*/resource_378_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
