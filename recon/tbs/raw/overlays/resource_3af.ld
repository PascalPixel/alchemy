/* resource_3af: its listing with maintained code
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_3af_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/FLY_BY_21.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/FLY_BY_22.o"(.text)
        "*/resource_3af_overlay.o"(.text.x0200889c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/IDLE_TURN.o"(.text)
        "*/resource_3af_overlay.o"(.text.x02008938)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/DECK_STATE_EVENT.o"(.text)
        "*/resource_3af_overlay.o"(.text.x02008ca0)
        "*/resource_3af_overlay.o"(.text.x02009444)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/RANDOM_ACTORS.o"(.text)
        "*/resource_3af_overlay.o"(.text.x020097a0)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/DECK_ACTORS.o"(.text)
        "*/resource_3af_overlay.o"(.text.x02009a98)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/DECK_EVENT_SCRIPT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/DECK_CREW_SCENE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/DECK_SEQ.o"(.text)
        "*/resource_3af_overlay.o"(.text.x0200a618)
        "*/resource_3af_overlay.o"(.text.x0200a9d4)
        "*/games/THE BROKEN SEAL/SRC/FIELD/FUNE_KANPAN/PARTY_ROSTER.o"(.text)
        "*/resource_3af_overlay.o"(.text.x0200b150)
        "*/resource_3af_overlay.o"(.text.x0200b1a8)
        "*/resource_3af_overlay.o"(.text.x0200b284)
        "*/resource_3af_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
