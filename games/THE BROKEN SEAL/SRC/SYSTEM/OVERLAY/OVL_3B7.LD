EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_IZUMI/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_IZUMI/IZUMI.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_IZUMI/PLAY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_IZUMI/ACTORS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_IZUMI/RIDE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_IZUMI/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_3b7_overlay.o"(.rodata.x02009a08)
    }
    .bss ALIGN(16) (NOLOAD) :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_IZUMI/RIDE.o"(.data .bss COMMON)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
