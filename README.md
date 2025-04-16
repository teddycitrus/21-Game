# 21-Game
The terminal-based '21 Card Game' I made in C++ for PointClickCare’s Early Tech Talent Incubator Software Engineering Summer Program.

> Note: This program makes heavy use of ANSI escape codes (clearing the console, applying text colors). It is advised you run the program in a terminal/terminal emulator that supports ANSI escape codes
> A recommended method of running the program is [this Replit console](https://replit.com/@johnmannul/21-Game?v=1).

# Gameplay
Note: These screenshots are taken on the Replit console, where the program was run.

The game is entirely-terminal based. It takes typed user input at numerous points.

![image](https://github.com/user-attachments/assets/0f6c73df-4e2a-4572-8628-9bb1d48e38b8)

Input lines are color-coded using ANSI escape codes.

![image](https://github.com/user-attachments/assets/31ec0f4b-a22f-484b-89d9-7f5275d384b4)

Apart from the name prompt at the start, all options/actions are display in a numbered list. Users make their choice by entering the corresponding number.

![image](https://github.com/user-attachments/assets/d95d2f10-2eaf-4a55-aba2-d497e0f0211a)

At most points, users will receive a 'PRESS ENTER TO CONTINUE' prompt. It takes whatever keys are pressed, along with the enter key, as input and moves on. The 'input' is discarded

![image](https://github.com/user-attachments/assets/33df49df-7452-4e24-ad8d-282793e048e2)

In each game, after a move is made, this display is refreshed.

![image](https://github.com/user-attachments/assets/cabd3ee6-2ebd-4a1b-a2e0-d3c8511cbd38)

When an ending has been determined, a relevant ending message is displayed.

![image](https://github.com/user-attachments/assets/aed6b228-d2ca-4080-87f8-8b02007a42e3)

After an ending, the replay prompt is delivered. Choosing 'no' terminates the program. Choosing 'yes' resets the game state and starts a new game.

![image](https://github.com/user-attachments/assets/30a007ff-d1ae-4b76-8491-1587620b4dad)


# Setup options (in order of recommendation)
1. [Replit](https://replit.com/@johnmannul/21-Game?v=1)
2. Copy-pasting ```src/main.cpp``` into an online C++ compiler (e.g. [Programiz](https://www.programiz.com/cpp-programming/online-compiler/)), then running the code
3. Download a C++ compiler (e.g. g++ from [MinGW](https://github.com/niXman/mingw-builds-binaries/releases) for Windows). Compile the program in cmd/terminal (e.g. ```g++ main.cpp -o main```), then execute (e.g. ```.\main.exe```)





**I hope you enjoy my 21 Game. Have fun!**
