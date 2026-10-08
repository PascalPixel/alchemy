EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/haidia_mura/entry.o"(.text)
        "*/games/tbs/src/field/common/object/staged_actor.o"(.text)
        "*/games/tbs/src/field/haidia_mura/mura.o"(.text)
        "*/games/tbs/src/field/world_map/linked_effects.o"(.text)
        "*/games/tbs/src/field/haidia_mura/import.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/games/tbs/src/field/haidia_mura/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
