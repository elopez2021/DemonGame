# DemonGame

## Game Overview

1. **Player Start**: The player, represented by the symbol `@`, begins in the corner of the screen.
2. **Boundary Limits**: The player cannot move off the screen.
3. **Movement Controls**: Use the numeric keypad to navigate:
   - `1`: Diagonal left-down
   - `2`: Move down
   - `3`: Diagonal right-down
   - `4`: Move left
   - `6`: Move right
   - `7`: Diagonal left-up
   - `8`: Move up
   - `9`: Diagonal right-up
4. **Demon Appearance**: At the start of the game, four demons (represented by `D`) will spawn randomly on the map. They move randomly and only when the player moves.
5. **Danger Zone**: Demons can kill the player if they get within three steps or less.
6. **Impenetrable Walls**: Both demons and the player are blocked by walls (a function will handle collision detection).
7. **Increasing Challenge**: Every 10 steps, a new demon appears on the map.
8. **Shield Power-Up**: Every 20 steps, a shield (`S`) will appear. If not picked up, it will last for 10 steps. Once picked up, it will disappear after 5 steps.

