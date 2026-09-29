/* resource_372: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. The overlay's own work follows the image. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_ARASHI/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_ARASHI/GROUP_DEPARTURE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_ARASHI/DORA_PLAZA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_ARASHI/STORM_SCENE_00D5C.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_ARASHI/GROUP_DEPARTURE_B.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_ARASHI/EVENT_SEQUENCE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_ARASHI/GROUP_DEPARTURE_C.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_ARASHI/SECOND_EVENT_SEQUENCE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_ARASHI/GROUP_DEPARTURE_D.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_ARASHI/GROUP_DEPARTURE_J.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_ARASHI/CALL_OUT_SEQUENCE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_ARASHI/GROUP_DEPARTURE_E.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_ARASHI/GROUP_DEPARTURE_K.o"(.text)
        "*/resource_372_overlay.o"(.text.x0200a180)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_ARASHI/RIVER_SEARCH.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_ARASHI/GROUP_DEPARTURE_I.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_ARASHI/STORM_SCENE_02DEC.o"(.text)
        "*/resource_372_overlay.o"(.text.x0200b1ac)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_ARASHI/GROUP_DEPARTURE_F.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_ARASHI/PULSING_GLOW.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_ARASHI/GROUP_DEPARTURE_G.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_ARASHI/SPAWN_PAIR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_ARASHI/GROUP_DEPARTURE_H.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_ARASHI/IMPORT.o"(.text)
        "*/resource_372_overlay.o"(.rodata)
    }
    .bss (NOLOAD) :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_ARASHI/GROUP_DEPARTURE_K.o"(.bss COMMON)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
