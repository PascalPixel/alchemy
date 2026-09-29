/* resource_3ae: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KAREI_TOREBI/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KAREI_TOREBI/VALUES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KAREI_TOREBI/GET_ENTRANCES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KAREI_TOREBI/SCENE_DATA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KAREI_TOREBI/GET_PLACEMENTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KAREI_TOREBI/SCENE_144.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KAREI_TOREBI/GET_EVENTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KAREI_TOREBI/DIALOGUE.o"(.text)
        "*/resource_3ae_overlay.o"(.text.x02008328)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KAREI_TOREBI/DIALOGUE2.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KAREI_TOREBI/LEAVE_SCENE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KAREI_TOREBI/SCENE_INIT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KAREI_TOREBI/SCENES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KAREI_TOREBI/FLAGGED_ACTORS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KAREI_TOREBI/SEQUENCE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KAREI_TOREBI/IMPORT.o"(.text)
        "*/resource_3ae_overlay.o"(.rodata.part1)
    }
    .bss : { "*/resource_3ae_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
