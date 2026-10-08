SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/common/haidia_babi/entry.o"(.text)
        "*/games/common/src/field/common/object/facing.o"(.text)
        "*/games/tbs/src/field/common/haidia_babi/babi.o"(.text)
        "*/games/tbs/src/field/common/haidia_babi/import.o"(.text)
        "*/games/tbs/src/field/common/haidia_babi/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
