/* resource_3b4: its entry veneers, its listing with maintained code and
   the compiler library linked in place, as pret links objects and libgcc
   members in ROM order. Its import veneers follow its code. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_ASHIBA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/OBJECT/STAGED_ACTOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_ASHIBA/STAGED_STEP.o"(.text)
        "*/resource_3b4_overlay.o"(.text.x020089f0)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_ASHIBA/RETURN_ZERO.o"(.text)
        "*/resource_3b4_overlay.o"(.text.x02008a50)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_ASHIBA/EMPTY_HOOK.o"(.text)
        "*/resource_3b4_overlay.o"(.text.x02008ee0)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_ASHIBA/CALL_HELPER3500.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_ASHIBA/OPEN_PASSAGE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_ASHIBA/RUN_SCENE3B4.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_ASHIBA/RAISE_TRIGGER.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_ASHIBA/RUN_SUPPLEMENTAL.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_ASHIBA/ACTOR_ELEVEN_FOLLOW.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_ASHIBA/TRIGGER_COLUMN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_ASHIBA/BRANCH_ON.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_ASHIBA/BLOCK_RECT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_ASHIBA/RUN_THREE.o"(.text)
        "*/resource_3b4_overlay.o"(.text.x02009f78)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_ASHIBA/ITEM_ICONS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KOROSSEO/ITEM_ICON.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_ASHIBA/RUN_2.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_ASHIBA/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_3b4_overlay.o"(.rodata.part1)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
