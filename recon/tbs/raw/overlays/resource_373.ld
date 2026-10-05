EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_MURA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/OBJECT/STAGED_ACTOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_MURA/MURA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/LINKED_EFFECTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_MURA/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_373_overlay.o"(.rodata.x0200e190)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
