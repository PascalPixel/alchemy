SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/common/yama_rama/entry.o"(.text)
        "*/games/tbs/src/field/common/yama_rama/rama.o"(.text)
        "*/games/tbs/src/field/common/yama_rama/import.o"(.text)
        "*/games/tbs/src/field/common/yama_rama/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
