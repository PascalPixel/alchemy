SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/common/imiru_fuchin/entry.o"(.text)
        "*/games/tbs/src/field/common/imiru_fuchin/fuchin.o"(.text)
        "*/games/tbs/src/field/common/imiru_fuchin/import.o"(.text)
        "*/games/tbs/src/field/common/imiru_fuchin/data.o"(.data)
    }
    .bss (NOLOAD) :
    {
        "*/games/tbs/src/field/common/imiru_fuchin/fuchin.o"(.bss COMMON)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
