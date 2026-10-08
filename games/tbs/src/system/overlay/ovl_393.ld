EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/korima_kawa/entry.o"(.text)
        "*/games/tbs/src/field/common/object/staged_actor.o"(.text)
        "*/games/tbs/src/field/korima_kawa/staged_palette.o"(.text)
        "*/games/tbs/src/field/common/korima/palette.o"(.text)
        "*/games/tbs/src/field/korima_kawa/import.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/games/tbs/src/field/korima_kawa/data.o"(.data)
    }
    .bss : { "*/resource_393_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
