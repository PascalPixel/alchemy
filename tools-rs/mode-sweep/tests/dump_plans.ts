// Parity oracle for tests/parity.rs.
//
// It prints ONE named section of tools/lib/mode_sweep.ts's planning output as
// canonical JSON, so the Rust side can compare strings and needs no JSON
// parser. It only IMPORTS from the TypeScript -- it modifies nothing.
//
// Usage: bun tests/dump_plans.ts <section>
import { canonicalJson } from "../../../tools/lib/canonical_json.ts";
import {
  MODES,
  FORK_MODES,
  STOCK_SWITCHES,
  singleConfigs,
  pairConfigs,
  rankedPairConfigs,
  tripleConfigs,
  historicalFamilyFactorialConfigs,
} from "../../../tools/lib/mode_sweep.ts";

const singleRow = (id: string, evidence?: Record<string, unknown>) => ({
  config: { ids: [id], flags: [], remove_flags: [], compiler_family: "routed" },
  cache_key: "",
  cached: false,
  compiled: true,
  ...(evidence ? { evidence } : {}),
});
const evidence = (halfwords: number, exact: boolean) => ({
  differing_halfwords: halfwords,
  size_delta: 0,
  exact,
  exact_size: true,
  instruction_order_proxy: false,
  register_allocation_proxy: 0,
  literal_placement_proxy: false,
  control_flow_proxy: false,
});

const sections: Record<string, () => unknown> = {
  fork_modes: () => FORK_MODES,
  stock_switches: () => STOCK_SWITCHES,
  mode_ids: () => MODES.map((mode) => mode.id),
  counts: () => [MODES.length, FORK_MODES.length, STOCK_SWITCHES.length],
  singles: () => singleConfigs(),
  pairs: () => pairConfigs(),
  pairs_17: () => pairConfigs(17),
  triples: () => tripleConfigs(new Set(["sched-postreload-off", "cse-gcse-off"]), 200),
  family_factorial: () => historicalFamilyFactorialConfigs(),
  // No evidence anywhere: every quality is Infinity, so every comparison is
  // Infinity - Infinity = NaN, which JavaScript's `||` treats as falsy and
  // falls through to the localeCompare tiebreak. This is the case a
  // partial_cmp-based port gets wrong.
  ranked_unevidenced: () => rankedPairConfigs(MODES.map((mode) => singleRow(mode.id)) as never, 64),
  ranked_one_exact: () =>
    rankedPairConfigs(
      MODES.map((mode) => singleRow(mode.id, evidence(100, mode.id === "cse-gcse-off"))) as never,
      64,
    ),
  ranked_graded: () =>
    rankedPairConfigs(
      MODES.map((mode, index) => singleRow(mode.id, evidence((index * 7) % 23, false))) as never,
      64,
    ),
  // Negative control: one mode id perturbed. Consumed by the Rust side only as
  // a difference count.
  ranked_graded_perturbed: () =>
    rankedPairConfigs(
      MODES.map((mode, index) =>
        singleRow(mode.id, evidence(mode.id === "cse-gcse-off" ? 0 : ((index * 7) % 23) + 1, false)),
      ) as never,
      64,
    ),
};

const name = Bun.argv[2] ?? "";
const section = sections[name];
if (!section) {
  console.error(`unknown section: ${name}; known: ${Object.keys(sections).join(", ")}`);
  process.exit(2);
}
console.log(canonicalJson(section()));
