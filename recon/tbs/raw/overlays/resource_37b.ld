/* resource_37b: its listing with maintained code linked in place, as pret
   links objects in ROM order. The overlay's own work follows the image. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_37b_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_SEKIZO/SETUP_STAGED_ACTORS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_SEKIZO/STAGED_ACTOR_SCENE.o"(.text)
        "*/resource_37b_overlay.o"(.text.x02008c8c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_SEKIZO/TILE_TRIGGER_0166C.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_SEKIZO/TILE_TRIGGER_016A4.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_SEKIZO/STATUE_DROP_SCENE.o"(.text)
        "*/resource_37b_overlay.o"(.text.x0200995c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_SEKIZO/EVENT_SEQUENCE.o"(.text)
        "*/resource_37b_overlay.o"(.text.x02009be8)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_SEKIZO/OPEN_SEAL.o"(.text)
        "*/resource_37b_overlay.o"(.text.x0200a244)
        "*/resource_37b_overlay.o"(.rodata)
    }
    .bss ALIGN(16) (NOLOAD) :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_SEKIZO/OPEN_SEAL.o"(.bss COMMON)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
