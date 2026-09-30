/* resource_396: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_HEYA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_HEYA/HEYA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_HEYA/TABLES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_HEYA/FLOOR_SWITCH.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_HEYA/STEPS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_HEYA/TABLE_SCENE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_HEYA/ENCOUNTER.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_HEYA/ENTER_ROOM.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_HEYA/PALETTE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_HEYA/TINT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_HEYA/GESTURE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_HEYA/SWIRL_SHRINK.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_HEYA/SPAWN_SPARKS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_HEYA/MAP_PATCH.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_HEYA/PALETTE_FADE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_HEYA/LANDING_DUST.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_HEYA/IMPORT.o"(.text)
        "*/resource_396_overlay.o"(.rodata)
    }
    .bss : { "*/resource_396_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
    /* The compiler calls its division helpers by their ABI names; here they
       are the overlay's veneers into the resident IWRAM dividers, as in
       MAIN.LD. */
    __divsi3 = Engine_MathDivide;
    __udivsi3 = IwramUnsignedDivideEntry;
}
