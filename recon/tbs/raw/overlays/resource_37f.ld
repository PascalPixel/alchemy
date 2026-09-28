/* resource_37f: its listing with maintained code linked in place,
   as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_37f_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_IRIGUCHI/PUSH_BLOCK.o"(.text)
        "*/resource_37f_overlay.o"(.text.x020088f4)
        "*/resource_37f_overlay.o"(.rodata.part1)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
