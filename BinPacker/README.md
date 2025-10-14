# Bin Packer
Bin Packer was made because I got tired of manually grouping items into trips for my games (Space Engineers, Elite Dangerous, etc.).
Bin Packer can read lists from files and group them based on the available space for each trip.

## Input List Formatting
The list that is given to Bin Packer **MUST**
 - Use a ` - <Item Name>: <Item Count>` format for each item
 - Include a `Cargo Space: X` anywhere in the file. X should be the amount of space available for each trip.

## Output Formats
There is currently only one output style, that being "Block".
It shows each run and how much of each item is used in the runs.
It provides all the necessary information to complete all the trips.



The second style I want to add, "Flow", can be harder to understand, but it looks nice.
Instead of showing how much of each item is used in a run, it attempts to compact all the info to 1 line.
Here is an example of how it will look: (The cargo space was set to 260)
```
    - Non-Lethal Weapons:           33    \
    - Medical Diagnostic Equipment: 48     \
    - Survival Equipment:           56      > Run 01 (260)
    - Emergency Power Cells:        66     /
    - Power Generators:             69    / \ (remainder: 12)
    - Fruit and Vegetables:         91       > Run 02 (260)
    - Computer Components:          97      /
    - Food Cartridges:             130     / \ (remainder: 70)
    - Superconductors:             136        > Run 03 (260)
    - Evacuation Shelter:          204       / \ (remainder: 150)
    - Copper:                      423          \ Runs 04-05 (260) (150+110) (remainder: 53)
    - Surface Stabilisers:         582           \ Runs 06-07 (260*2) (remainder: 62)
    - Polymers:                    646            > Runs 08-10 (260*2) (115+126)
```
It uses `\` and `/` to group items into runs and `>` to show a run.
If a run that uses multiple items has a remainder, a `\` is added at the end to "pick up" the remainder and move it to the next run.
If a run that uses one item has a remainder, it uses a `\` instead of `>` to show the remainder flowing over to the next run.
As you can tell, Flow does not say how much of each item to use.
Instead, it only lists how many runs are needed, how full each run is, and if a run has a remainder.
