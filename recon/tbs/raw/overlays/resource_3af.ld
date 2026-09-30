/* resource_3af: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. The overlay's own work follows the image. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/UPDATE_SCALE.o"(.text)
        "*/resource_3af_overlay.o"(.text.x020080c4)
        "*/resource_3af_overlay.o"(.text.x020082ec)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/KANPAN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/KANPAN2.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/SLOT_MODES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/SELECT_TABLE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/SLOT_BANDS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/SLOT_POSITION.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/INIT_ACTOR_SLOTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/APPLY_FIVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/TOLBI_ARRIVAL.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/RUN_THREE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/ROBIN_TALK.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/RUN_3.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/IMPORT.o"(.text)
        "*/resource_3af_overlay.o"(.rodata)
    }
    .bss ALIGN(16) (NOLOAD) :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/KANPAN.o"(.data .bss COMMON)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/SLOT_MODES.o"(COMMON)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/KANPAN2.o"(COMMON)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
