/* resource_38a: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_SUIRO/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_SUIRO/OBJECTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text)
        "*/resource_38a_overlay.o"(.text.x02008314)
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_SUIRO/SCENE_EXITS.o"(.text)
        "*/resource_38a_overlay.o"(.text.x02008350)
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_SUIRO/CELL24.o"(.text)
        "*/resource_38a_overlay.o"(.text.x020083e4)
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_SUIRO/BURST.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_SUIRO/IMPORT.o"(.text)
        "*/resource_38a_overlay.o"(.rodata.part1)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
    /* The compiler calls its division helper by its ABI name; here it is the
       overlay's veneer into the resident IWRAM divider, as in MAIN.LD. */
    __divsi3 = IwramSignedDivideEntry;
}
