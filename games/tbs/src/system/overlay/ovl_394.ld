SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/korima_magari/entry.o"(.text)
        "*/games/tbs/src/field/korima_magari/magari.o"(.text)
        "*/games/tbs/src/field/common/map/panel.o"(.text)
        "*/games/tbs/src/field/korima_magari/puzzle.o"(.text)
        "*/games/tbs/src/field/common/korima/palette.o"(.text)
        "*/games/tbs/src/field/korima_magari/import.o"(.text)
        "*/games/tbs/src/field/korima_magari/data.o"(.data)
    }
    .bss : { "*/resource_394_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
