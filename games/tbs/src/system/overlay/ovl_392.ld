EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/korima_hiroba/entry.o"(.text)
        "*/games/tbs/src/field/common/object/staged_actor.o"(.text)
        "*/games/tbs/src/field/korima_hiroba/staged_particle.o"(.text)
        "*/games/tbs/src/field/common/korima/palette.o"(.text)
        "*/games/tbs/src/field/korima_hiroba/orbit.o"(.text)
        "*/games/tbs/src/field/korima_hiroba/import.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/games/tbs/src/field/korima_hiroba/data.o"(.data)
    }
    .bss : { "*/resource_392_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
