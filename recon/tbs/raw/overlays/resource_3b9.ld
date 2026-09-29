/* resource_3b9: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORASHIAMU_IRIGUCHI/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORASHIAMU_IRIGUCHI/GET_ENTRANCES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORASHIAMU_IRIGUCHI/TABLES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORASHIAMU_IRIGUCHI/GET_TABLES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORASHIAMU_IRIGUCHI/DIALOGUE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORASHIAMU_IRIGUCHI/BRANCHING_ACTOR_PRESENTATION.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORASHIAMU_IRIGUCHI/GATHER_SCENE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORASHIAMU_IRIGUCHI/STAGING.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORASHIAMU_IRIGUCHI/SCENE_INIT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORASHIAMU_IRIGUCHI/SELECTOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORASHIAMU_IRIGUCHI/IMPORT.o"(.text)
        "*/resource_3b9_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
