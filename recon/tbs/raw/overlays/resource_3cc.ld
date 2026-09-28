/* resource_3cc: its listing with maintained code linked in place, as pret
   links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_3cc_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/DEBUG/TEST_ROOMS/EMPTY.o"(.text)
        "*/resource_3cc_overlay.o"(.text.x020080b8)
        "*/resource_3cc_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
