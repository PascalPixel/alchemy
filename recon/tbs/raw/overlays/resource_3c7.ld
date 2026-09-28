/* resource_3c7: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_HEYA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_HEYA/POSE.o"(.text)
        "*/resource_3c7_overlay.o"(.text.x0200804c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_HEYA/EXITS.o"(.text)
        "*/resource_3c7_overlay.o"(.text.x02008084)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_HEYA/FACING_BRANCH.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_HEYA/ALTERNATE_FACING_BRANCH.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_HEYA/FACING_ACTION.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_HEYA/FACING_CUE_BRANCH.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_HEYA/POSE_SEQUENCE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_HEYA/SEQUENCE.o"(.text)
        "*/resource_3c7_overlay.o"(.text.x020084b0)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_HEYA/SCRIPT.o"(.text)
        "*/resource_3c7_overlay.o"(.text.x02008eec)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_HEYA/IMPORT.o"(.text)
        "*/resource_3c7_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
