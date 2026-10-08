SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/MENU/CLEAR/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/CLEAR/CLEAR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/CLEAR/TILEGRID.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/CLEAR/PASSWORD.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/CLEAR/IMPORT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/MENU/CLEAR/DATA.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
