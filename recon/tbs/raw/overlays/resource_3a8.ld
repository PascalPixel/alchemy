/* resource_3a8: its listing with maintained code linked in place,
   as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_3a8_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_MACHI/GROUP_FX.o"(.text)
        "*/resource_3a8_overlay.o"(.text.x020080ec)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_MACHI/GROUP_FX2.o"(.text)
        "*/resource_3a8_overlay.o"(.text.x02008180)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_MACHI/GROUP_FX3.o"(.text)
        "*/resource_3a8_overlay.o"(.text.x02008240)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_MACHI/GROUP_FX4.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_MACHI/EXIT_DOOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_MACHI/STAGED_GROUP.o"(.text)
        "*/resource_3a8_overlay.o"(.text.x02008590)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_MACHI/STAGED_GROUP2.o"(.text)
        "*/resource_3a8_overlay.o"(.text.x0200964c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_MACHI/ENTRY_ACTORS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_MACHI/MIDDLE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_MACHI/SPEED_UP_ACTORS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_MACHI/DANCE_ALTERNATION.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_MACHI/LATE.o"(.text)
        "*/resource_3a8_overlay.o"(.text.x0200bb14)
        "*/resource_3a8_overlay.o"(.rodata.part1)
    }
    .bss : { "*/resource_3a8_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
