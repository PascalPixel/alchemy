/* resource_37b: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. The overlay's own work follows the image. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_SEKIZO/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_SEKIZO/SCENE_DATA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_SEKIZO/SETUP_STAGED_ACTORS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_SEKIZO/STAGED_ACTOR_SCENE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_SEKIZO/SEAL_OPENED.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_SEKIZO/GUARD_SCENES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_SEKIZO/TILE_TRIGGER_0166C.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_SEKIZO/TILE_TRIGGER_016A4.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_SEKIZO/STATUE_DROP_SCENE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_SEKIZO/CELL_STEPS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_SEKIZO/EVENT_SEQUENCE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_SEKIZO/TILE_TABLE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_SEKIZO/OPEN_SEAL.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_SEKIZO/BEAMS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_SEKIZO/IMPORT.o"(.text)
        "*/resource_37b_overlay.o"(.rodata)
    }
    .bss ALIGN(16) (NOLOAD) :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_SEKIZO/OPEN_SEAL.o"(.bss COMMON)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_SEKIZO/GUARD_SCENES.o"(.bss COMMON)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
