/* resource_37f: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_IRIGUCHI/ENTRY.o"(.text)
        "*/resource_37f_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_IRIGUCHI/PUSH_BLOCK.o"(.text)
        "*/resource_37f_overlay.o"(.text.x020088f4)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_IRIGUCHI/IMPORT.o"(.text)
        "*/resource_37f_overlay.o"(.rodata.part1)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
