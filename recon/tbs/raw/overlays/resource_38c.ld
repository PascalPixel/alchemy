/* resource_38c: its listing with maintained code linked in place, as pret
   links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_38c_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_HEYA/MESSAGE_BRANCH.o"(.text)
        "*/resource_38c_overlay.o"(.text.x02008500)
        "*/resource_38c_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
