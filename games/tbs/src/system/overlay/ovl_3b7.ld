EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/torebi_izumi/entry.o"(.text)
        "*/games/tbs/src/field/torebi_izumi/izumi.o"(.text)
        "*/games/tbs/src/field/torebi_izumi/play.o"(.text)
        "*/games/tbs/src/field/torebi_izumi/actors.o"(.text)
        "*/games/tbs/src/field/torebi_izumi/ride.o"(.text)
        "*/games/tbs/src/field/torebi_izumi/import.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/games/tbs/src/field/torebi_izumi/data.o"(.data)
    }
    .bss ALIGN(16) (NOLOAD) :
    {
        "*/games/tbs/src/field/torebi_izumi/ride.o"(.data .bss COMMON)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
