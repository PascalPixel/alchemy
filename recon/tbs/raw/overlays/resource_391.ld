/* resource_391: its entry veneers, its maintained code and the compiler
   library linked in place, as pret links objects and libgcc members in ROM
   order. Its import veneers follow its code, and its work follows its
   loaded image. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_HASHI/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/OBJECT/STAGED_ACTOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_HASHI/BRANCHING_FORMATION.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_HASHI/FALLING_EFFECT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_HASHI/ALTERNATE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_HASHI/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_391_overlay.o"(.rodata.part1)
    }
    /* The overlay's own work, past its loaded image. */
    .bss (NOLOAD) :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_HASHI/FALLING_EFFECT.o"(.bss)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_HASHI/BRANCHING_FORMATION.o"(.bss)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
