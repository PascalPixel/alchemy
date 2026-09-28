/* resource_39d: its listing with maintained code and the compiler
   library linked in place, as pret links objects and libgcc
   members in ROM order. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_39d_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_CHOJO/OBJECTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/resource_39d_overlay.o"(.text.x0200813c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_CHOJO/TABLES.o"(.text)
        "*/resource_39d_overlay.o"(.text.x0200835c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_CHOJO/PRESENT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_CHOJO/INITIALIZE_MOTION.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_CHOJO/FORMATION.o"(.text)
        "*/resource_39d_overlay.o"(.text.x02008b24)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_CHOJO/SCENE59.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_CHOJO/SINK_PAIR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_CHOJO/RISE_PAIR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_CHOJO/COLLECT_SLOTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_CHOJO/LOWER_BAND.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_CHOJO/RAISE_BAND.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_CHOJO/ARC.o"(.text)
        "*/resource_39d_overlay.o"(.text.x0200b4bc)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_39d_overlay.o"(.rodata.part1)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
