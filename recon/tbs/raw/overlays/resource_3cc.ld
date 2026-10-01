/* resource_3cc: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/DEBUG/TEST_ROOMS/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/DEBUG/TEST_ROOMS/EMPTY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/DEBUG/TEST_ROOMS/IMPORT.o"(.text)
        "*/resource_3cc_overlay.o"(.rodata.x020080c8)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
