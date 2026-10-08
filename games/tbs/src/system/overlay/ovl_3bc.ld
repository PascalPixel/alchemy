EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/korosseo_maruta/entry.o"(.text)
        "*/games/tbs/src/field/korosseo_maruta/maruta.o"(.text)
        "*/games/tbs/src/field/common/korosseo/solo_competitor.o"(.text)
        "*/games/tbs/src/field/korosseo_maruta/obstacle.o"(.text)
        "*/games/tbs/src/field/common/korosseo/solo_round.o"(.text)
        "*/games/tbs/src/field/korosseo_maruta/interact.o"(.text)
        "*/games/tbs/src/field/common/korosseo/korosseo.o"(.text)
        "*/games/tbs/src/field/korosseo_maruta/transition.o"(.text)
        "*/games/tbs/src/field/common/korosseo/compete.o"(.text)
        "*/games/tbs/src/field/korosseo_maruta/motion_reset.o"(.text)
        "*/games/tbs/src/field/common/korosseo/marker.o"(.text)
        "*/games/tbs/src/field/korosseo_maruta/palette_task.o"(.text)
        "*/games/tbs/src/field/common/korosseo/item_icon.o"(.text)
        "*/games/tbs/src/field/korosseo_maruta/effect.o"(.text)
        "*/games/tbs/src/field/common/korosseo/path_rival.o"(.text)
        "*/games/tbs/src/field/korosseo_maruta/control.o"(.text)
        "*/games/tbs/src/field/common/korosseo/scene_state.o"(.text)
        "*/games/tbs/src/field/korosseo_maruta/push.o"(.text)
        "*/games/tbs/src/field/korosseo_maruta/import.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/games/tbs/src/field/korosseo_maruta/data.o"(.data)
        "*/games/tbs/src/graphics/korosseo/gauge.o"(.rodata)
        "*/games/tbs/src/field/korosseo_maruta/data2.o"(.data)
    }
    .bss (NOLOAD) : { "*/resource_3bc_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
