/* resource_381: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_FUNKA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_FUNKA/EXTENDED_EFFECT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_FUNKA/ERUPTION_RING.o"(.text)
        "*/resource_381_overlay.o"(.text.x02009084)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_FUNKA/EFFECT_PAIR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_FUNKA/STEPS.o"(.text)
        "*/resource_381_overlay.o"(.text.x0200b01c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_STAR/RING_SETUP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_FUNKA/IMPORT.o"(.text)
        "*/resource_381_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
