#!/usr/bin/env bun
// Renamed to alchemist.ts. This is a temporary compatibility forward for
// in-flight harvesting agents that were launched with this path baked into
// their instructions -- delete once the current harvesting wave completes
// and no live agent still calls this path.
import { main } from "./alchemist.ts";

if (import.meta.main) await main();
