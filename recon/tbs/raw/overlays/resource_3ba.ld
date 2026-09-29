/* resource_3ba: its entry veneers, its listing with maintained code and
   the compiler library linked in place, as pret links objects and libgcc
   members in ROM order. Its import veneers follow its code. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KAWA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KAWA/COORDINATOR.o"(.text)
        "*/resource_3ba_overlay.o"(.text.x02008540)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KAWA/APPLY_VALUE768.o"(.text)
        "*/resource_3ba_overlay.o"(.text.x02008840)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KAWA/RUN_BRANCHED.o"(.text)
        "*/resource_3ba_overlay.o"(.text.x02008a3c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KAWA/RUN_COMMAND.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KAWA/STAGE_INTRO.o"(.text)
        "*/resource_3ba_overlay.o"(.text.x020093e4)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KAWA/RUN_SCENE3BA.o"(.text)
        "*/resource_3ba_overlay.o"(.text.x02009910)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KAWA/SET_STATE.o"(.text)
        "*/resource_3ba_overlay.o"(.text.x02009b5c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KAWA/RUN_PARTY.o"(.text)
        "*/resource_3ba_overlay.o"(.text.x02009d64)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KAWA/RUN_MIDDLE.o"(.text)
        "*/resource_3ba_overlay.o"(.text.x0200a124)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KAWA/RUN_TWO.o"(.text)
        "*/resource_3ba_overlay.o"(.text.x0200a910)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KAWA/RESET_MOTION.o"(.text)
        "*/resource_3ba_overlay.o"(.text.x0200abec)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KAWA/RELEASE_TABLE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KOROSSEO/ITEM_ICON.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KAWA/WRITE_U32.o"(.text)
        "*/resource_3ba_overlay.o"(.text.x0200af94)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KAWA/SPAWN_KIND285.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KOROSSEO/PATH_RIVAL.o"(.text)
        "*/resource_3ba_overlay.o"(.text.x0200b3a0)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KAWA/BUILD_SCENE.o"(.text)
        "*/resource_3ba_overlay.o"(.text.x0200b84c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KOROSSEO/SCENE_STATE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KAWA/FIND_SLOT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KAWA/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_3ba_overlay.o"(.rodata.part1)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
