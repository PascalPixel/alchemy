/* resource_380: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_STAR/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_STAR/OBJECTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_STAR/STAR.o"(.text)
        "*/resource_380_overlay.o"(.text.x0200a27c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_STAR/PARTY_INTRO.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_STAR/ENTRY_STATE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_STAR/SAY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_STAR/PRESENT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_STAR/MYTHRIL.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_STAR/RISE_SPARKS.o"(.text)
        "*/resource_380_overlay.o"(.text.x0200c49c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_STAR/RING_SETUP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_STAR/IMPORT.o"(.text)
        "*/resource_380_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
    /* The compiler calls its division helper by its ABI name; here it is the
       overlay's veneer into the resident IWRAM divider, as in MAIN.LD. */
    __divsi3 = IwramSignedDivideEntry;
}
