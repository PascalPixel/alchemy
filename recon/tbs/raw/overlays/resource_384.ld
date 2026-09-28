/* resource_384: its listing with maintained code linked in place, as pret
   links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_384_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KUUPUAPPU_RUNPA/JAIL.o"(.text)
        "*/resource_384_overlay.o"(.text.x02008260)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KUUPUAPPU_RUNPA/JAIL.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
