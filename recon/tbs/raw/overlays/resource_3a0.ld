/* resource_3a0: its entry veneers, its listing with maintained code and
   the compiler library linked in place, as pret links objects and libgcc
   members in ROM order. Its import veneers follow its code. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/SHIAN_MURA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SHIAN_MURA/MURA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text.effect_spawn)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SHIAN_MURA/MURA2.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SHIAN_MURA/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_3a0_overlay.o"(.rodata.x020094fc)
    }
    /DISCARD/ : { "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text.effect_spawn_resident) *(.ARM.attributes) *(.comment) *(.note*) }
}
