/* resource_3bf: its entry veneers, its listing with maintained code and
   the compiler library linked in place, as pret links objects and libgcc
   members in ROM order. Its import veneers follow its code. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/OBJECT/STAGED_ACTOR.o"(.text)
        "*/resource_3bf_overlay.o"(.text.x020089dc)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/EMPTY_DATA.o"(.text)
        "*/resource_3bf_overlay.o"(.text.x02008a34)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/BRIDGE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/SET_POSITION_PAIRS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/CELLS.o"(.text)
        "*/resource_3bf_overlay.o"(.text.x02009150)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/FORTRESS.o"(.text)
        "*/resource_3bf_overlay.o"(.text.x02009ab0)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/TURN.o"(.text)
        "*/resource_3bf_overlay.o"(.text.x02009c4c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/GUARD_LINES.o"(.text)
        "*/resource_3bf_overlay.o"(.text.x02009e94)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/SEQUENCE_TAIL.o"(.text)
        "*/resource_3bf_overlay.o"(.text.x0200a134)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/INSPECT.o"(.text)
        "*/resource_3bf_overlay.o"(.text.x0200a7b0)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/ACTOR21.o"(.text)
        "*/resource_3bf_overlay.o"(.text.x0200c704)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/VARIANTS.o"(.text)
        "*/resource_3bf_overlay.o"(.text.x0200cba4)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/ACTOR_LINES.o"(.text)
        "*/resource_3bf_overlay.o"(.text.x0200cbfc)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/ACTOR20.o"(.text)
        "*/resource_3bf_overlay.o"(.text.x0200cd50)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/ACTOR13.o"(.text)
        "*/resource_3bf_overlay.o"(.text.x0200cda0)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/SETUP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_3bf_overlay.o"(.rodata.part1)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
