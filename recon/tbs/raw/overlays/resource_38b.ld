/* resource_38b: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. The overlay's own work follows the image. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_MURA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_MURA/FACING.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_MURA/GET_TABLES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_MURA/TABLES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_MURA/GET_PLACEMENTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_MURA/STEPS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_MURA/GET_EVENTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_MURA/SCENES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_MURA/SCENE_INIT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_MURA/PUZZLE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_MURA/PUSH_BLOCK.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_MURA/CORNER.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_MURA/TILES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_MURA/IMPORT.o"(.text)
        "*/resource_38b_overlay.o"(.rodata)
    }
    .bss (NOLOAD) :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_MURA/CORNER.o"(.bss COMMON)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
