SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/debug/item_level/entry.o"(.text)
        "*/games/tbs/src/debug/item_level/level.o"(.text)
        "*/games/tbs/src/debug/item_level/import.o"(.text)
        "*/games/tbs/src/debug/item_level/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
