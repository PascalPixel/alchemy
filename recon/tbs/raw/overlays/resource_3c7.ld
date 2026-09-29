/* resource_3c7: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_HEYA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_HEYA/POSE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_HEYA/REGIONS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_HEYA/EXITS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_HEYA/GET_PLACEMENTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_HEYA/ITEM_SHOP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_HEYA/FACING_BRANCH.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_HEYA/ALTERNATE_FACING_BRANCH.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_HEYA/FACING_ACTION.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_HEYA/FACING_CUE_BRANCH.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_HEYA/POSE_SEQUENCE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_HEYA/SEQUENCE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_HEYA/GET_EVENTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_HEYA/SCRIPT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_HEYA/TELL_OTHERS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_HEYA/SCENE_INIT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_HEYA/IMPORT.o"(.text)
        "*/resource_3c7_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
