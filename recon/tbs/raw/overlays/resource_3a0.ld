/* resource_3a0: its entry veneers, its listing with maintained code and
   the compiler library linked in place, as pret links objects and libgcc
   members in ROM order. Its import veneers follow its code. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/SHIAN_MURA/ENTRY.o"(.text)
        "*/resource_3a0_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/resource_3a0_overlay.o"(.text.x0200813c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SHIAN_MURA/PRESENTATION_FLAG.o"(.text)
        "*/resource_3a0_overlay.o"(.text.x02008358)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SHIAN_MURA/ACTOR_TRACKING.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SHIAN_MURA/GATE_WATCH.o"(.text)
        "*/resource_3a0_overlay.o"(.text.x0200859c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SHIAN_MURA/NPC_MEET.o"(.text)
        "*/resource_3a0_overlay.o"(.text.x02008cec)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SHIAN_MURA/LEADER_HOP.o"(.text)
        "*/resource_3a0_overlay.o"(.text.x02008de8)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SHIAN_MURA/ACTOR19_SCRIPT.o"(.text)
        "*/resource_3a0_overlay.o"(.text.x02009060)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SHIAN_MURA/SETUP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SHIAN_MURA/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_3a0_overlay.o"(.rodata.part1)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
