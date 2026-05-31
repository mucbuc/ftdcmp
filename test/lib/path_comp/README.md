# Loop segment indexing

## Overview

A `Loop` stores two parallel containers:

- `points` — a flat list of all control points and endpoints
- `segments` — a list of end indices, one per segment, using half-open ranges

The first point in `points` is always the move-to origin and is never referenced by a segment index. Each segment describes the path from its predecessor's endpoint to its own endpoint.

## Half-open index convention

Segment indices follow the same convention as C++ iterators: a segment owns the range `[begin, end)` where `end` is the stored index and `begin` is the previous segment's `end` (or `1` for the first segment).

```
segment_size = segment_end - segment_begin
```

The size determines the SVG command:

| size | points in range | SVG command |
|------|-----------------|-------------|
| 1    | endpoint only   | `L` (line)  |
| 2    | control + endpoint | `Q` (quadratic) |
| 3    | control1 + control2 + endpoint | `C` (cubic) |

## Example — triangle

```cpp
Loop<float> loop { {0, 0} };   // points: [{0,0}]        segments: []
loop.line({100, 0});            // points: [{0,0},{100,0}] segments: [2]
loop.line({50, 100});           // points: [...,{50,100}]  segments: [2, 3]
loop.close();                   // points: [...,{0,0}]     segments: [2, 3, 5]
```

After construction:

```
index:  0       1        2        3      4
point: (0,0)  (100,0)  (50,100) (0,0)  [past-end]
              |________|         |_____|
              seg[0]=2           seg[2]=5
                       |_________|
                       seg[1]=3
```

The closing segment has `end = points.size() + 1`. The renderer wraps the index with `% points.size()`, landing back on point 0 — completing the loop as an `L` command back to the origin.

## Rendering walkthrough

```cpp
auto segment_begin = 1;
for (auto segment_end : loop.segments()) {
    const auto size = segment_end - segment_begin;
    // write SVG command based on size, reading points[segment_begin..segment_end)
    segment_begin = segment_end;
}
```

For the triangle above this produces:

```
M 0 0        ← move-to: points[0]
L 100 0      ← size=1: points[1]
L 50 100     ← size=1: points[2]
L 0 0        ← size=1: points[4 % 4] = points[0]  (close)
```

## Builder methods and their effect on segments

| call | points pushed | segment index pushed |
|------|--------------|----------------------|
| `Loop(begin)` | `begin` | — |
| `line(dest)` | `dest` | `points.size()` |
| `curve(ctrl, dest)` | `ctrl`, `dest` | `points.size()` |
| `curve(c1, c2, dest)` | `c1`, `c2`, `dest` | `points.size()` |
| `close()` | `begin` (copy of points[0]) | `points.size() + 1` |

The `+1` in `close()` is intentional: it makes `segment_size = 1` for the closing segment without requiring the renderer to special-case it, while the `% points.size()` wrap in the renderer resolves the out-of-bounds index back to point 0.
