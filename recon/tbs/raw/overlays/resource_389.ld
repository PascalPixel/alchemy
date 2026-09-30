/* resource_389: its entry veneers, its listing with maintained code and
   the compiler library linked in place, as pret links objects and libgcc
   members in ROM order. Its import veneers follow its code. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_HASHIRA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/OBJECT/STAGED_ACTOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_HASHIRA/HASHIRA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_HASHIRA/HASHIRA2.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_HASHIRA/SEQUENCE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_HASHIRA/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_389_overlay.o"(.rodata.part1)
    }
    /* The overlay's own work, past its loaded image. */
    .bss (NOLOAD) : { *(.bss COMMON) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
