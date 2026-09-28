/* resource_389: its listing with maintained code and the compiler library
   linked in place, as pret links objects and libgcc members in ROM order. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_389_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/OBJECT/STAGED_ACTOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_HASHIRA/EFFECT_STEP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_HASHIRA/SPAWN_PILLAR_EFFECT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_HASHIRA/STAGED_PLACEMENT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_HASHIRA/PILLAR_DROP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_HASHIRA/ACTOR13_IDLE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_HASHIRA/SEQUENCE.o"(.text)
        "*/resource_389_overlay.o"(.text.x020093a8)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_389_overlay.o"(.rodata.part1)
    }
    /* The overlay's own work, past its loaded image. */
    .bss (NOLOAD) : { *(.bss COMMON) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
