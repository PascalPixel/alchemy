/* resource_3ac: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code, and the interiors' tables follow those. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/RUNPA_SUHARA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/RUNPA_SUHARA/INTERIORS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/RUNPA_SUHARA/MERCHANT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/RUNPA_SUHARA/SHOPS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/RUNPA_SUHARA/SCENE_INIT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/RUNPA_SUHARA/PRIEST.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/RUNPA_SUHARA/IMPORT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/RUNPA_SUHARA/INTERIORS.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
