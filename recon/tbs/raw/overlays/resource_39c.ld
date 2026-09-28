/* resource_39c: its entry veneers, its listing with maintained code and
   the compiler library linked in place, as pret links objects and libgcc
   members in ROM order. Its import veneers follow its code. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/ENTRY.o"(.text)
        "*/resource_39c_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/resource_39c_overlay.o"(.text.x0200813c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/OBJECT/STAGED_ACTOR.o"(.text)
        "*/resource_39c_overlay.o"(.text.x02008cc0)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/PUSHED_BLOCK_SCENE.o"(.text)
        "*/resource_39c_overlay.o"(.text.x02009340)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/FLOOR_SWITCH.o"(.text)
        "*/resource_39c_overlay.o"(.text.x02009d3c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/PUSH_PILLAR.o"(.text)
        "*/resource_39c_overlay.o"(.text.x0200a2c4)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/COLUMN_PROBE_SCENE.o"(.text)
        "*/resource_39c_overlay.o"(.text.x0200a504)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/BLOCK_FLAGS.o"(.text)
        "*/resource_39c_overlay.o"(.text.x0200a600)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/COLUMN_CHOREOGRAPHY.o"(.text)
        "*/resource_39c_overlay.o"(.text.x0200adcc)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/ACTOR_SINK.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/RANDOM_EFFECT_SEQUENCE.o"(.text)
        "*/resource_39c_overlay.o"(.text.x0200b5b8)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/OPEN_STAIR.o"(.text)
        "*/resource_39c_overlay.o"(.text.x0200ba64)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/MAKYURI/SPARK_PAIR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_IRIGUCHI/RAISE_PILLAR.o"(.text)
        "*/resource_39c_overlay.o"(.text.x0200d578)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/MAKYURI/SPAWN_TIMER.o"(.text)
        "*/resource_39c_overlay.o"(.text.x0200d948)
        "*/games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_39c_overlay.o"(.rodata.part1)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
