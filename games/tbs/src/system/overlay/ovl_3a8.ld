SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/karei_machi/entry.o"(.text)
        "*/games/tbs/src/field/karei_machi/machi.o"(.text)
        "*/games/tbs/src/field/karei_machi/import.o"(.text)
        "*/games/tbs/src/field/karei_machi/data.o"(.data)
    }
    .bss ALIGN(8) : { "*/resource_3a8_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
