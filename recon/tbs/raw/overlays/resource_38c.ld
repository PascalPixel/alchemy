/* resource_38c: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_HEYA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_HEYA/MESSAGE_BRANCH.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_HEYA/IMPORT.o"(.text)
        "*/resource_38c_overlay.o"(.rodata.x02008598)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
