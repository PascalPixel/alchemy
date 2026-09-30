/* resource_371: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. The overlay's own work follows the image. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/MAP.o"(.text)
        "*/resource_371_overlay.o"(.text.x02009ca4)
        "*/games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/CHASE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/BLACK_ORB.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/SITE_138.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/EXIT_TRIGGER.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/REWARD.o"(.text)
        "*/resource_371_overlay.o"(.text.x0200a8e8)
        "*/games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/MAP3.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/DISPLAY_TRANSITION.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/EFFECTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/LINKED_EFFECTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/IMPORT.o"(.text)
        "*/resource_371_overlay.o"(.rodata)
    }
    .bss (NOLOAD) :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/REWARD.o"(.bss COMMON)
        "*/games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/DISPLAY_TRANSITION.o"(.bss COMMON)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
