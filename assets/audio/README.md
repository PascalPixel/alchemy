# Sound sources

The ROM-side engine checks the little-endian `Smsh` signature and drives a
relocated mixed Thumb/ARM sample renderer. This identifies an engine-native
sequence, tone, and signed-PCM pipeline; MIDI and rendered audio are useful
previews, but neither is an exact source format by itself.

The current local evidence divides the native data as follows:

| ROM range | Evidence-backed role | Current source state |
|---|---|---|
| `080fb7a0..080fba77` | command-dispatch and pitch/volume lookup tables | unclaimed |
| `080fba78..080fc4ff` | tone-bank area; song headers select roots at `080fba78` and `080fc138` | unclaimed |
| `080fc500..080fc623` | engine configuration and lookup data | unclaimed |
| `080fc624..080fc683` | eight music-player records | unclaimed |
| `080fc684..080fd043` | 312-entry sound-selection table | `song_table.json` |
| `080fd044..080fd047` | shared empty sound header | unclaimed |
| `080fd048..0815fb77` | tone-referenced wave arena, including 32 signed-PCM records | unclaimed |
| `0815fb78..08184697` | native sequence arena selected by the sound table | 241 complete units claimed below |

`song_table.json` uses symbols for header references and records the player
selected by the ROM code. Its final halfword always duplicates that selector;
the source represents the proven duplication without inventing a meaning for
the field. Forty-eight empty entries share `sound_empty`, and repeated real
headers likewise reuse one symbol.

`sequences/` contains 241 independently selected units: 535 tracks, 81,732
events, and 124,340 byte-verified bytes. `index.json` records every exact range
and the 19 headers still rejected by the semantic decoder. Those failures stay
unclaimed until their additional running-status form or nonzero separator can
be represented; they are never stored as opaque ROM slices.

The sequence codec preserves loops, pattern calls, repeats, running-status
omission, priorities, reverb, tone-bank references, and pointer targets. A
MIDI export may later be generated from the native documents, but must never
become the input used to rebuild the ROM. Tone banks and signed-PCM records
remain separate recovery units.
