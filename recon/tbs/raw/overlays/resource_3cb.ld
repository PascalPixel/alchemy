/* resource_3cb: its entry veneers, its listing with maintained code and
   the compiler library linked in place, as pret links objects and libgcc
   members in ROM order. Its import veneers follow its code. The lobby's own
   counters follow the image. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/LOBBY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/LOBBY3.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/LOBBY2.o"(.text)
        "*/resource_3cb_overlay.o"(.text.x02008580)
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/LOBBY4.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/WINS_TALK.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/CHOICE_PROMPT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/APPLY_VALUE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/PARTY_HAS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/LOBBY5.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/ROUND.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_3cb_overlay.o"(.rodata.x02009928)
    }
    .bss (NOLOAD) :
    {
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/LOBBY3.o"(.bss)
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/LOBBY2.o"(.bss)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
