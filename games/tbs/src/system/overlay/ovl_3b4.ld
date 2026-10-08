EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/takara_ashiba/entry.o"(.text)
        "*/games/tbs/src/field/common/object/staged_actor.o"(.text)
        "*/games/tbs/src/field/takara_ashiba/ashiba.o"(.text)
        "*/games/tbs/src/field/common/korosseo/item_icon.o"(.text)
        "*/games/tbs/src/field/takara_ashiba/setup.o"(.text)
        "*/games/tbs/src/field/takara_ashiba/import.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/games/tbs/src/field/takara_ashiba/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
