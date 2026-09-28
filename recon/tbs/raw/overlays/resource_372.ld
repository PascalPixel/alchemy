/* resource_372: its listing with maintained code
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_372_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_ARASHI/STORM_SCENE_00D5C.o"(.text)
        "*/resource_372_overlay.o"(.text.x02008ec4)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_ARASHI/EVENT_SEQUENCE.o"(.text)
        "*/resource_372_overlay.o"(.text.x020090a4)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_ARASHI/SECOND_EVENT_SEQUENCE.o"(.text)
        "*/resource_372_overlay.o"(.text.x02009498)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_ARASHI/STORM_SCENE_02DEC.o"(.text)
        "*/resource_372_overlay.o"(.text.x0200b1ac)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_ARASHI/PULSING_GLOW.o"(.text)
        "*/resource_372_overlay.o"(.text.x0200be18)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_ARASHI/SPAWN_PAIR.o"(.text)
        "*/resource_372_overlay.o"(.text.x0200c550)
        "*/resource_372_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
