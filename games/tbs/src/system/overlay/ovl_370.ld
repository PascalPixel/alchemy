SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/menu/clear/entry.o"(.text)
        "*/games/tbs/src/menu/clear/clear.o"(.text)
        "*/games/tbs/src/menu/clear/import.o"(.text)
        "*/games/tbs/src/menu/clear/clear.o"(.data)
        "*/games/tbs/src/menu/clear/keybd.o"(.data)
        "*/games/tbs/src/menu/clear/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
