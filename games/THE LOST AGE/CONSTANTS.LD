/* Shared sound-driver settings are source constants in COMMON/SRC/SOUND/SOUND.C. */
/* Routines copied to RAM as one block across several modules: the length
   of each copy runs from its entry to the end label of its last module. */
Resource_Decode2CodeSize = Resource_Decode2End - Resource_Decode2Code;
