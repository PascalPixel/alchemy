EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/arutamira_dou/entry.o"(.text)
        "*/games/tbs/src/field/common/object/staged_actor.o"(.text)
        "*/games/tbs/src/field/arutamira_dou/dou.o"(.text)
        "*/games/tbs/src/field/arutamira_dou/import.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/games/tbs/src/field/arutamira_dou/data.o"(.data)
    }
    .bss : { "*/games/tbs/src/field/arutamira_dou/dou.o"(.bss COMMON) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
