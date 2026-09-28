/* resource_3a8: its listing with maintained code linked in place,
   as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_3a8_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_MACHI/EXIT_DOOR.o"(.text)
        "*/resource_3a8_overlay.o"(.text.x02008504)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_MACHI/ENTRY_ACTORS.o"(.text)
        "*/resource_3a8_overlay.o"(.text.x020097e8)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_MACHI/SPEED_UP_ACTORS.o"(.text)
        "*/resource_3a8_overlay.o"(.text.x02009f14)
        "*/resource_3a8_overlay.o"(.rodata.part1)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
