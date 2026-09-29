/* resource_38b: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. The overlay's own work follows the image. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_MURA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_MURA/FACING.o"(.text)
        "*/resource_38b_overlay.o"(.text.x02008088)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_MURA/TABLES.o"(.text)
        "*/resource_38b_overlay.o"(.text.x0200811c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_MURA/STEPS.o"(.text)
        "*/resource_38b_overlay.o"(.text.x02008198)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_MURA/SCENES.o"(.text)
        "*/resource_38b_overlay.o"(.text.x02008890)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_MURA/PUZZLE.o"(.text)
        "*/resource_38b_overlay.o"(.text.x02008db4)
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
