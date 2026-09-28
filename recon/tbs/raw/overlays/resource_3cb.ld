/* resource_3cb: its entry veneers, its listing with maintained code and
   the compiler library linked in place, as pret links objects and libgcc
   members in ROM order. Its import veneers follow its code. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/TABLES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/PEER_SLOT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/WRITE_SLOT.o"(.text)
        "*/resource_3cb_overlay.o"(.text.x02008148)
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/CLEAR_FLAGS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/RECEIVE_PARTY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/COPY_ENTRIES.o"(.text)
        "*/resource_3cb_overlay.o"(.text.x02008580)
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/WINS_TALK.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/CHOICE_PROMPT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/APPLY_VALUE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/PARTY_HAS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/NO_OP.o"(.text)
        "*/resource_3cb_overlay.o"(.text.x02008f8c)
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/COUNT_MESSAGE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/EVENTS.o"(.text)
        "*/resource_3cb_overlay.o"(.text.x020091bc)
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_3cb_overlay.o"(.rodata.part1)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
