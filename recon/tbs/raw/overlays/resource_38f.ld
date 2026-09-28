/* resource_38f: its listing with maintained code linked in place, as pret
   links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_38f_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MURA/ACTOR_FX.o"(.text)
        "*/resource_38f_overlay.o"(.text.x0200816c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MURA/ACTOR_LINES.o"(.text)
        "*/resource_38f_overlay.o"(.text.x02008284)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MURA/EXIT_WALK.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MURA/SWITCH_ANIMATE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MURA/OBJECT_SPREAD_SCENE.o"(.text)
        "*/resource_38f_overlay.o"(.text.x02008694)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MURA/EXTENDED_SEQUENCE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MURA/REGION_TRIGGER.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_HASHI/FALLING_EFFECT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MURA/FLAG_BRANCH.o"(.text)
        "*/resource_38f_overlay.o"(.text.x0200a99c)
        "*/resource_38f_overlay.o"(.rodata)
    }
    /* The overlay's own work, past its loaded image. */
    .bss (NOLOAD) :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_HASHI/FALLING_EFFECT.o"(.bss)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MURA/ACTOR_FX.o"(.bss)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
