SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/arutin_yama/entry.o"(.text)
        "*/games/tbs/src/field/arutin_yama/yama.o"(.text)
        "*/games/tbs/src/field/arutin_yama/import.o"(.text)
        "*/games/tbs/src/field/arutin_yama/data.o"(.data)
    }
    .bss : { "*/resource_3a4_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
