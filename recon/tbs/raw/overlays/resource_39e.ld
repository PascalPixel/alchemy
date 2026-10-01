/* resource_39e: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHIAN_JIIN/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHIAN_JIIN/NEW_EFFECT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text.effect_spawn)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHIAN_JIIN/JIIN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHIAN_JIIN/IMPORT.o"(.text)
        "*/resource_39e_overlay.o"(.rodata.x0200c584)
    }
    /DISCARD/ : { "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text.effect_spawn_resident) *(.ARM.attributes) *(.comment) *(.note*) }
}
