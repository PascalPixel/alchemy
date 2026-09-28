/* resource_3b2: its listing with maintained code and the compiler library linked in place,
   as pret links objects and libgcc members in ROM order. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_3b2_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/OBJECT/STAGED_ACTOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/EFFOWNER.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/resource_3b2_overlay.o"(.text.x02008ae8)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/PROLOGUE.o"(.text)
        "*/resource_3b2_overlay.o"(.text.x02008cc4)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/PROLOGUE2.o"(.text)
        "*/resource_3b2_overlay.o"(.text.x02008d48)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/PROLOGUE3.o"(.text)
        "*/resource_3b2_overlay.o"(.text.x02008ec4)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/PROLOGUE4.o"(.text)
        "*/resource_3b2_overlay.o"(.text.x02008f70)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/PROLOGUE5.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/ESCAPE_COLUMN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/SCRIPT37.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/AREA_ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/ISLAND_AND_CAVES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/SETTLE_BLOCK_NORTH.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/ESCAPE18.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/SETTLE_BLOCK_NORTH_B.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/ESCAPE9.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/SETTLE_BLOCK_WEST.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/SETTLE_BLOCK.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_SHIMA/ESCAPE_B.o"(.text)
        "*/resource_3b2_overlay.o"(.text.x0200afb0)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_3b2_overlay.o"(.rodata.part1)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
