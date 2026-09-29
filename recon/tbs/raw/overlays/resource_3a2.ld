/* resource_3a2: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/YAMA_RAMA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/YAMA_RAMA/ACTOR_SETUP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/YAMA_RAMA/GET_ENTRANCES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/YAMA_RAMA/SCENE_EXITS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/YAMA_RAMA/GET_PLACEMENTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/YAMA_RAMA/EVENT_SETUP_SCENE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/YAMA_RAMA/GET_EVENTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/YAMA_RAMA/ENTRY_STATE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/YAMA_RAMA/IMPORT.o"(.text)
        "*/resource_3a2_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
