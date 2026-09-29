/* resource_373: its entry veneers, its listing with maintained code and
   the compiler library linked in place, as pret links objects and libgcc
   members in ROM order. Its import veneers follow its code. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_MURA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/OBJECT/STAGED_ACTOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_MURA/STAGED_MOTION.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_MURA/CAMERA_RISE_SCENE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_MURA/PUPPY_BARKS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_MURA/TABLE_PAIR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_MURA/REPAIR_MORNING.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_MURA/ENTRY_STATE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_MURA/GERALD_VISITS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_MURA/ACTORS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_MURA/WALK_SCENE_032B0.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_MURA/WALK_SCENE_03380.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_MURA/RESET_RUN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_MURA/REPAIR_OPENING.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_MURA/STAGING.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_MURA/FACING.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_MURA/DUST_BURST.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_MURA/OPEN_LANE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_MURA/ORBIT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/LINKED_EFFECTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_MURA/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_373_overlay.o"(.rodata.part1)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
    /* The compiler calls its remainder helper by its ABI name; here it is the
       overlay's veneer into the resident IWRAM divider, as in MAIN.LD. */
    __umodsi3 = IwramUnsignedRemainderEntry;
}
