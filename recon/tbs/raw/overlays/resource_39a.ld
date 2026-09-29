/* resource_39a: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. The overlay's own work follows the image. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/IMIRU_FUCHIN/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/IMIRU_FUCHIN/OPENING.o"(.text)
        "*/resource_39a_overlay.o"(.text.x02008050)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/IMIRU_FUCHIN/HOOKS.o"(.text)
        "*/resource_39a_overlay.o"(.text.x020080f8)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/IMIRU_FUCHIN/TRANSITION.o"(.text)
        "*/resource_39a_overlay.o"(.text.x02008f30)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/IMIRU_FUCHIN/LAYOUT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/IMIRU_FUCHIN/TRACKING.o"(.text)
        "*/resource_39a_overlay.o"(.text.x02009750)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/IMIRU_FUCHIN/ARRIVAL.o"(.text)
        "*/resource_39a_overlay.o"(.text.x020097e4)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/IMIRU_FUCHIN/DRAGON_EYE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/IMIRU_FUCHIN/SPAWN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/IMIRU_FUCHIN/CAVE_DUST.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/IMIRU_FUCHIN/CALLBACKS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/IMIRU_FUCHIN/FADE_IN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/IMIRU_FUCHIN/HEADING.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/IMIRU_FUCHIN/IMPORT.o"(.text)
        "*/resource_39a_overlay.o"(.rodata)
    }
    .bss (NOLOAD) :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/IMIRU_FUCHIN/TRACKING.o"(.bss COMMON)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
