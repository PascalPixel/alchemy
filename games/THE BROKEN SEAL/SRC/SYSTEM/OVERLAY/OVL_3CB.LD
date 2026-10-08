EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/LOBBY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/LOBBY.o"(.rodata)
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/DATA.o"(.data)
    }
    .bss (NOLOAD) :
    {
        "*/games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/LOBBY.o"(.bss)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
