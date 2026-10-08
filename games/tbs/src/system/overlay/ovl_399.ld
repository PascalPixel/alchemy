SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/imiru_mura/entry.o"(.text)
        "*/games/tbs/src/field/imiru_mura/mura.o"(.text)
        "*/games/tbs/src/field/imiru_mura/import.o"(.text)
        "*/games/tbs/src/field/imiru_mura/data.o"(.data)
    }
    .bss : { "*/resource_399_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
