EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/goma_hashira/entry.o"(.text)
        "*/games/tbs/src/field/common/object/staged_actor.o"(.text)
        "*/games/tbs/src/field/goma_hashira/hashira.o"(.text)
        "*/games/tbs/src/field/goma_hashira/pillar.o"(.text)
        "*/games/tbs/src/field/goma_hashira/sequence.o"(.text)
        "*/games/tbs/src/field/goma_hashira/import.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/games/tbs/src/field/goma_hashira/data.o"(.data)
    }
    .bss (NOLOAD) : { *(.bss COMMON) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
