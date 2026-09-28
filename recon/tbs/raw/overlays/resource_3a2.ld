/* resource_3a2: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/YAMA_RAMA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/YAMA_RAMA/ACTOR_SETUP.o"(.text)
        "*/resource_3a2_overlay.o"(.text.x0200807c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/YAMA_RAMA/SCENE_EXITS.o"(.text)
        "*/resource_3a2_overlay.o"(.text.x020080b8)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/YAMA_RAMA/EVENT_SETUP_SCENE.o"(.text)
        "*/resource_3a2_overlay.o"(.text.x02009180)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/YAMA_RAMA/IMPORT.o"(.text)
        "*/resource_3a2_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
