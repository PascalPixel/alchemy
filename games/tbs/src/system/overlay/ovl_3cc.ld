SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/debug/test_rooms/entry.o"(.text)
        "*/games/tbs/src/debug/test_rooms/empty.o"(.text)
        "*/games/tbs/src/debug/test_rooms/import.o"(.text)
        "*/games/tbs/src/debug/test_rooms/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
