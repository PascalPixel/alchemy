EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/korosseo_kawa/entry.o"(.text)
        "*/games/tbs/src/field/korosseo_kawa/kawa.o"(.text)
        "*/games/tbs/src/field/common/korosseo/solo_competitor.o"(.text)
        "*/games/tbs/src/field/korosseo_kawa/set_state.o"(.text)
        "*/games/tbs/src/field/common/korosseo/solo_round.o"(.text)
        "*/games/tbs/src/field/korosseo_kawa/interact.o"(.text)
        "*/games/tbs/src/field/common/korosseo/korosseo.o"(.text)
        "*/games/tbs/src/field/korosseo_kawa/scene.o"(.text)
        "*/games/tbs/src/field/common/korosseo/compete.o"(.text)
        "*/games/tbs/src/field/korosseo_kawa/reset_motion.o"(.text)
        "*/games/tbs/src/field/common/korosseo/marker.o"(.text)
        "*/games/tbs/src/field/korosseo_kawa/setup.o"(.text)
        "*/games/tbs/src/field/common/korosseo/item_icon.o"(.text)
        "*/games/tbs/src/field/korosseo_kawa/effect.o"(.text)
        "*/games/tbs/src/field/common/korosseo/path_rival.o"(.text)
        "*/games/tbs/src/field/korosseo_kawa/control.o"(.text)
        "*/games/tbs/src/field/common/korosseo/scene_state.o"(.text)
        "*/games/tbs/src/field/korosseo_kawa/find_slot.o"(.text)
        "*/games/tbs/src/field/korosseo_kawa/import.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/games/tbs/src/field/korosseo_kawa/data.o"(.data)
        "*/games/tbs/src/graphics/korosseo/gauge.o"(.rodata)
        "*/games/tbs/src/field/korosseo_kawa/data2.o"(.data)
    }
    .bss : { "*/resource_3ba_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
