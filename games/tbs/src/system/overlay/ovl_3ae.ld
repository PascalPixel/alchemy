SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/common/karei_torebi/entry.o"(.text)
        "*/games/tbs/src/field/common/karei_torebi/torebi.o"(.text)
        "*/games/tbs/src/field/common/karei_torebi/import.o"(.text)
        "*/games/tbs/src/field/common/karei_torebi/data.o"(.data)
    }
    .bss : { "*/resource_3ae_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
