/* resource_3b2: its listing with maintained code and the compiler library linked in place,
   as pret links objects and libgcc members in ROM order. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_3b2_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/OBJECT/STAGED_ACTOR.o"(.text)
        "*/resource_3b2_overlay.o"(.text.x020089dc)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/resource_3b2_overlay.o"(.text.x02008ae8)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/ESCAPE_COLUMN.o"(.text)
        "*/resource_3b2_overlay.o"(.text.x02009374)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/AREA_ENTRY.o"(.text)
        "*/resource_3b2_overlay.o"(.text.x02009494)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/SETTLE_BLOCK_NORTH.o"(.text)
        "*/resource_3b2_overlay.o"(.text.x0200a5f0)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/SETTLE_BLOCK_NORTH_B.o"(.text)
        "*/resource_3b2_overlay.o"(.text.x0200a848)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/SETTLE_BLOCK_WEST.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/SETTLE_BLOCK.o"(.text)
        "*/resource_3b2_overlay.o"(.text.x0200ab80)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_3b2_overlay.o"(.rodata.part1)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
