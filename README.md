# Wi-Fi Planner Starter (Minimal)

This is a *minimal* starter program for the upcoming Wi-Fi coverage / greedy-algorithms lab.

It reads the cleaned OpenStreetMap point dataset produced by `osm_wifi_prep`:

```
id,name,lat,lon,x_m,y_m
way:322703800,Mather Hall,40.3789,-82.3957,61.1248,454.442
...
```

Then it runs a **solver** (currently dummy solvers), with the solver call wrapped in a timer.

Later you'll replace/add solvers, then output map-friendly data.

## Build

```bash
make
```

## Run

```bash
./wifi_planner_starter kenyonout.csv --radius 60 --solver one_at_first
```

Available solvers:
- `none` — returns 0 access points (baseline)
- `one_at_first` — places 1 AP at the first point
- `random_k` — picks k random points as APs

Example:

```bash
./wifi_planner_starter kenyonout.csv --radius 80 --solver random_k --k 10 --seed 42
```

## What “radius” means

This is a simplified geometric model:
- Each demand point is a location (x_m,y_m)
- An AP covers a demand point if the distance is <= `radius`

This ignores walls/interference/attenuation on purpose.

## Suggested next enhancements (for the full project)

1. **Add a greedy solver**
   - Example direction: place APs to maximize newly-covered demand points per placement.
   - Then improve: add tie-breakers, stop conditions, and better candidate sets.

2. **Add an exact baseline (small N)**
   - For small subsets (e.g., 25 buildings), brute force over subsets of candidate AP placements.
   - Use this to test whether your greedy heuristic matches optimal on small instances.

3. **Output map-friendly artifacts**
   - Write `aps.csv` or `aps.geojson` so you can visualize AP locations.
   - Later: generate a small HTML page that plots points + AP circles.

## Notes

- This code is intentionally simple and not yet “smart.”
- The important part right now is the pipeline:
  CSV in -> solver call -> solution object -> reporting
