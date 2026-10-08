EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/korima_hashi/entry.o"(.text)
        "*/games/tbs/src/field/common/object/staged_actor.o"(.text)
        "*/games/tbs/src/field/korima_hashi/branching_formation.o"(.text)
        "*/games/tbs/src/field/korima_hashi/falling_effect.o"(.text)
        "*/games/tbs/src/field/korima_hashi/alternate.o"(.text)
        "*/games/tbs/src/field/korima_hashi/import.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/games/tbs/src/field/korima_hashi/data.o"(.data)
    }
    .bss (NOLOAD) :
    {
        "*/games/tbs/src/field/korima_hashi/falling_effect.o"(.bss)
        "*/games/tbs/src/field/korima_hashi/branching_formation.o"(.bss)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
