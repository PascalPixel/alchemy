EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/LOBBY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/TRANSFER.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/CONNECT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/WINS_TALK.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/DIALOGUE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/LOBBY.o"(.rodata)
        "*/resource_3cb_overlay.o"(.rodata.x02009928)
    }
    .bss (NOLOAD) :
    {
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/LOBBY.o"(.bss)
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/TRANSFER.o"(.bss)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
