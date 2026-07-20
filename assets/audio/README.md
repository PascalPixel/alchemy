# Sound sources

The ROM-side engine checks the little-endian `Smsh` signature and drives a
relocated mixed Thumb/ARM sample renderer. This identifies an engine-native
sequence, tone, and signed-PCM pipeline; MIDI and rendered audio are useful
previews, but neither is an exact source format by itself.

The current local evidence divides the native data as follows:

| ROM range | Evidence-backed role | Current source state |
|---|---|---|
| `080fb792..080fba77` | alignment, command dispatch, and pitch/volume lookup tables | `engine/seigyo.json` |
| `080fba78..080fc503` | 225 tone records in banks rooted at `080fba78` and `080fc138` | `engine/onshoku.json` |
| `080fc504..080fc623` | eighteen 16-byte CGB waveforms | `engine/hakei.json` |
| `080fc624..080fc683` | eight music-player records | `engine/saisei.json` |
| `080fc684..080fd043` | 312-entry sound-selection table | `song_table.json` |
| `080fd044..080fd047` | shared empty sound header | `residuals/index.json` |
| `080fd048..0815fb77` | tone-referenced wave arena, including 32 signed-PCM records | 32 PCM records claimed; five synthesizer descriptors remain |
| `0815fb78..08184697` | native sequence arena selected by the sound table | 260 `.mid` + deviation sidecars claimed below |

`song_table.json` uses symbols for header references and records the player
selected by the ROM code. Its final halfword always duplicates that selector;
the source represents the proven duplication without inventing a meaning for
the field. Forty-eight empty entries share `sound_empty`, and repeated real
headers likewise reuse one symbol.

`midi/` and `data/` hold all 260 independently selected nonempty units. Each
unit is a tracked Standard MIDI file `midi/sound_NNN.mid` (the canonical core:
notes as note-on/off, waits as delta-time, engine control and structural
commands as verbatim markers) plus, only when the ROM's byte encoding departs
from the codec's default rules, a minimal deviation sidecar
`data/sound_NNN.json`. The default rules are greedy running-status (reuse the
active status whenever legal), note-parameter omission (drop trailing key or
velocity equal to the running value), and greedy largest-first wait-table
splits; these match the ROM's dominant encoding, so 163 of the 260 units carry
no sidecar at all. A sidecar records per-event exceptions addressed by stable
`(track, event-index)` references into the default stream, and each track
entry fingerprints the default stream's event count and hash so a `.mid` that
drifts from its sidecar fails loudly. `midi/index.json` records every exact
range. `tools/midi_sequence.ts` is the codec (`.mid` + sidecar → engine
bytes), the converter (`convert-all`), and the validator (`validate-all`);
`tools/midi_roundtrip.ts` holds the underlying sequence-JSON↔SMF mapping and
its documented losses. Stream and header padding follows absolute ROM
alignment, including units whose first track is not word-aligned.

`waves/` contains the 32 tone-referenced signed-PCM records as canonical mono
8-bit WAV sources plus an index retaining the engine's exact fixed-point
frequency and loop position. The builder reverses WAV's unsigned 8-bit bias,
recreates the native 16-byte header, and checks zero alignment. The five
zero-length synthesizer descriptors between the PCM banks are not mislabeled
as audio samples and remain unclaimed. The sound-table exporter assigns the
`sound_empty` symbol only after proving that its complete 32-bit header word is
zero, so `residuals/index.json` retains those four bytes as typed header fields.

The `.mid` + sidecar pair rebuilds the exact engine bytes, preserving loops,
pattern calls, repeats, running-status omission, priorities, reverb, tone-bank
references, and pointer targets. The MIDI carries the canonical musical core
and the sidecar carries only the ROM's non-canonical encoding choices, so the
two together are the byte-exact source: neither the MIDI nor a rendered audio
preview is a source on its own. Tone banks and signed-PCM records remain
separate recovery units.

## Song usage ledger

`../data/song_usage.json` (`{format:1, entries:[{song_id, symbol, name,
evidence}]}`) tracks the effort to give the 260 sequence songs usage-derived
reconstruction labels. Each entry is keyed by the song's index into the
312-entry `song_table.json` and its `sound_NNN` symbol. A song is renamed only
when it is provably played in exactly one named game context; otherwise it
keeps `sound_NNN` and records `name: null`. The `evidence` line states what the
trace found.

Current state: every entry is `null`. The play mechanism is fully decoded, the
context is not. Game code plays a song by passing its table index (low 12 bits
of the request word) to the sound dispatcher `Func_080f9080` through the veneer
`Func_080f9010`; 574 such call sites resolve to 68 distinct song indices in the
current tree. But those call sites live in address-named functions with no
in-repo reference that ties them to a bank-9 place name (`World Map`, `Vale
Sanctum`, `Venus Lighthouse`, ...) or a bank-10 scene code (`TITLE`, `WORLD`,
...), and no map, encounter, or scene table in `assets/` carries a song-id
field. With no decodable link from a song id to a named context, no label can
be proven, so none is invented. The ledger records the per-song play-site facts
so labels can be filled in once the semantic callers are decompiled and named.
