/* resource_3ab: its listing with maintained code linked in place, as pret
   links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_3ab_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_MURA/EFFECT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text)
        "*/resource_3ab_overlay.o"(.text.x02008314)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_MURA/REGIONS.o"(.text)
        "*/resource_3ab_overlay.o"(.text.x02008350)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_MURA/SECRETS.o"(.text)
        "*/resource_3ab_overlay.o"(.text.x020086e4)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_MURA/TALK.o"(.text)
        "*/resource_3ab_overlay.o"(.text.x020088ec)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_MURA/GUARDS.o"(.text)
        "*/resource_3ab_overlay.o"(.text.x02008a90)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_MURA/GATE.o"(.text)
        "*/resource_3ab_overlay.o"(.text.x02008ff0)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_MURA/ESCAPE.o"(.text)
        "*/resource_3ab_overlay.o"(.text.x02009668)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_MURA/NUT.o"(.text)
        "*/resource_3ab_overlay.o"(.text.x02009918)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_MURA/EFFECT.o"(.rodata)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_MURA/VILLAGE.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
    /* The compiler calls its division helper by its ABI name; here it is the
       overlay's veneer into the resident IWRAM divider, as in MAIN.LD. */
    __divsi3 = IwramSignedDivideEntry;
}
