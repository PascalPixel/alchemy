/* resource_39e: its listing with maintained code linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_39e_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHIAN_JIIN/NEW_EFFECT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/resource_39e_overlay.o"(.text.x0200813c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHIAN_JIIN/XIAN_ACTOR.o"(.text)
        "*/resource_39e_overlay.o"(.text.x0200834c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHIAN_JIIN/XIAN_DATA.o"(.text)
        "*/resource_39e_overlay.o"(.text.x02008388)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHIAN_JIIN/FLAG_BRANCH.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHIAN_JIIN/TEMPLE_WALK.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHIAN_JIIN/SEQUENCE_B.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHIAN_JIIN/SPIN_AWAY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHIAN_JIIN/DESCENT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHIAN_JIIN/ACTOR_TRANSITION.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHIAN_JIIN/WALK_BY_FACING.o"(.text)
        "*/resource_39e_overlay.o"(.text.x0200902c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHIAN_JIIN/GATHERING_SCENE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHIAN_JIIN/ENSEMBLE.o"(.text)
        "*/resource_39e_overlay.o"(.text.x0200a574)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHIAN_JIIN/PARTICLES.o"(.text)
        "*/resource_39e_overlay.o"(.text.x0200a7ec)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHIAN_JIIN/ROOF_ENSEMBLE.o"(.text)
        "*/resource_39e_overlay.o"(.text.x0200be58)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHIAN_JIIN/FINALE.o"(.text)
        "*/resource_39e_overlay.o"(.text.x0200c33c)
        "*/resource_39e_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
