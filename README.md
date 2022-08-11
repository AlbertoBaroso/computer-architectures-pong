# Computer Architecture - Pong

Single/Multi-player pong game developed on the LandTiger (LPC1768 ARM Development Board) for the Computer Architecture course @ PoliTo.

The aim of this project is to dive into the low-level details of a computer architecture and learn how to use its components: clock frequencies, timers, interrupts, memory, AD/DA conversions, LEDs, buzzer, LCD...

![Gameplay](./media/gameplay.gif)

## Gameplay

The player can horizontally move a paddle through the potentiometer available on the LandTiger board.

The ball can bounce on red walls (5px thick) on the left, upper, and right portions of the display.

Whenever the ball hits a wall the buzzer emits a lower pitched note, a higher pitch note is played when it hits the paddle.

The user can start the game by pressing the button KEY1.
To pause the game and resume it later, the user should press the button KEY2.
In case of game-over the player should press the INT0 button to prepare a new game, followed by KEY1 to start it.

### v1

Single player version.
Every time the ball hits the paddle, the score is incremented. The score is printed on
the left side of the screen, and it's saved in between games, the new record is displayed in the top right corner. The initial record to beat is 100 points.
If the ball falls below the paddle, the message “You lose” is displayed.

### v2

Multi-player version.
The top wall is replaced by a top paddle driven by the CPU.
This second paddle is fully automatic and, when the play starts, it only moves horizontally at a constant speed.

Every time the ball falls below one's paddle, the other player’s score is incremented by 1.
The game ends as soon as one player scores 5 points, with the message “You win” or “You lose” displayed on
the screen for both players.
The score is printed on the left side of the screen for the first player and on the right side for the second player.

## LandTiger specifications

LPC1768 MCU Cortex-M3 core:

- Clock frequency up to 100MHz.
- Built-in Nested Vectored Interrupt Controller (NVIC).
- 512KB on-chip Flash program memory.
- Single 3.3V power supply (2.4V - 3.6V).
- 2 CAN bus communication interfaces.
- DAC output interface, on-board speaker and speaker output driver (LM386).
- ADC input interface, on-board adjustable potentiometer input.
- Color LCD display interface (supports 2.8-inch or 3.2-inch color TFT 320X240).
- 2 user keys, 2 function keys (INT0 and Reset button).
- 8 LED lights (Digital outputs).
- Five-way joystick button (Digital Inputs).
