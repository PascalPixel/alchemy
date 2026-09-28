/* resource_3ae: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KAREI_TOREBI/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KAREI_TOREBI/VALUES.o"(.text)
        "*/resource_3ae_overlay.o"(.text.x02008040)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KAREI_TOREBI/SCENE_DATA.o"(.text)
        "*/resource_3ae_overlay.o"(.text.x020080a0)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KAREI_TOREBI/SCENE_144.o"(.text)
        "*/resource_3ae_overlay.o"(.text.x0200819c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KAREI_TOREBI/DIALOGUE.o"(.text)
        "*/resource_3ae_overlay.o"(.text.x02008328)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KAREI_TOREBI/DIALOGUE2.o"(.text)
        "*/resource_3ae_overlay.o"(.text.x020087dc)
        "*/resource_3ae_overlay.o"(.text.x0200886c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KAREI_TOREBI/SCENES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KAREI_TOREBI/FLAGGED_ACTORS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KAREI_TOREBI/SEQUENCE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KAREI_TOREBI/IMPORT.o"(.text)
        "*/resource_3ae_overlay.o"(.rodata.part1)
    }
    .bss : { "*/resource_3ae_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
