EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/RAMAKAN_SABAKU/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RAMAKAN_SABAKU/SABAKU.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text.effect_spawn)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RAMAKAN_SABAKU/SETUP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RAMAKAN_SABAKU/TRAVEL.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RAMAKAN_SABAKU/STORM.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RAMAKAN_SABAKU/SAND_SETUP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RAMAKAN_SABAKU/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RAMAKAN_SABAKU/DATA.o"(.data)
    }
    .bss ALIGN(16) (NOLOAD) : { "*/resource_3a5_overlay.o"(.bss) }
    /DISCARD/ : { "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text.effect_spawn_resident) *(.ARM.attributes) *(.comment) *(.note*) }
}
