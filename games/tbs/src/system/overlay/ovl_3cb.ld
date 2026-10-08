EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/menu/link_lobby/entry.o"(.text)
        "*/games/tbs/src/menu/link_lobby/lobby.o"(.text)
        "*/games/tbs/src/menu/link_lobby/import.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/games/tbs/src/menu/link_lobby/lobby.o"(.rodata)
        "*/games/tbs/src/menu/link_lobby/data.o"(.data)
    }
    .bss (NOLOAD) :
    {
        "*/games/tbs/src/menu/link_lobby/lobby.o"(.bss)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
