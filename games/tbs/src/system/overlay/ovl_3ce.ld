SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/debug/menu_test/entry.o"(.text)
        "*/games/tbs/src/debug/menu_test/test.o"(.text)
        "*/games/tbs/src/debug/menu_test/import.o"(.text)
        "*/games/tbs/src/debug/menu_test/data.o"(.data)
    }
    .bss : { "*/resource_3ce_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
