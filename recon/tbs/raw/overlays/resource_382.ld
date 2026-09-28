/* resource_382: its listing with maintained code and the compiler library
   linked in place, as pret links objects and libgcc members in ROM order. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_382_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_MURA/OBJECT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/resource_382_overlay.o"(.text.x0200813c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_MURA/ACTOR_PRESENTATION.o"(.text)
        "*/resource_382_overlay.o"(.text.x020088cc)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_MURA/SCENE_STEP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_MURA/SCENE_CELLS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_MURA/SEQUENCE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_MURA/ENTRY_STATE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_MURA/FACING_RANGE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_MURA/DRIFT.o"(.text)
        "*/resource_382_overlay.o"(.text.x02009900)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_382_overlay.o"(.rodata.part1)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
