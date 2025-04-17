<a id="readme-top"></a>
<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li><a href="#about-the-project">About The Project</a></li>
    <li><a href="#getting-started">Getting Started</a></li>
    <li><a href="#gameplay">Gameplay</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#license">License</a></li>
  </ol>
</details>

## About The Project
This project was a terminal-based '21 Card Game' I made in C++ for PointClickCare’s Early Tech Talent Incubator Software Engineering Summer Program.

> Note: This program makes heavy use of ANSI escape codes (clearing the console, applying text colors). It is advised you run the program in a terminal/terminal emulator that supports ANSI escape codes. 
> A recommended method of running the program is [this Replit console](https://replit.com/@teddycitroos/21-Game?v=1).

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Getting started
### Prerequisites
* Download a C++ compiler (e.g. g++ from [MinGW](https://github.com/niXman/mingw-builds-binaries/releases) for Windows)

### Compiling
* Compile the program in cmd/terminal using valid syntax for the compiler you have installed.
* g++ example:
  ```sh
  g++ main.cpp -o main
  ```

### Execution
* Execute the program in cmd/terminal using valid syntax for the compiler you have installed.
* g++ example:
  ```sh
  .\main.exe
  ```

### Simpler alternatives
1. [Replit](https://replit.com/@teddycitroos/21-Game?v=1)
2. Copy-pasting ```src/main.cpp``` into an online C++ compiler (e.g. [Programiz](https://www.programiz.com/cpp-programming/online-compiler/)), then running the code

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Gameplay
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

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- ROADMAP -->
## Roadmap
### Completed
- [x] Initial terminal-based game loop
  - [x] Player vs Dealer logic
  - [x] Dealer turn logic
  - [x] Basic win/loss/tie resolution
  - [x] Replay option/prompt
- [x] Hand system
  - [x] Card drawing & deck management
  - [x] Statistics tracker (win/loss count, hand total)
- [x] AI Players
  - [x] 'Enemy' struct, objects, functions
  - [x] Enemy stay/draw card logic 
- [x] 'No instant wins' rule toggle + logic
- [x] Terminal UI
  - [x] Console-clearing with ANSI escape codes/system calls
  - [x] Color-coded text input and output via ANSI escape codes
- [x] Refactor into modular code

### Future Developments
- [ ] Implement save/load functionality for persistent game state (web version could leverage session/local storage)
- [ ] Difficulty modes for AI enemies; 'confidence', 'optimism', etc.
- [ ] GUI version using C++ & libraries like SFML or Qt, or Java & Swing, or Unity Game Engine, etc.
- [ ] Sound support (voicelines, sound effects)  
- [ ] Networked multiplayer support (likely in a webapp)
- [ ] Custom rule editor (e.g. double down, insurance, etc.)  
- [ ] Card-counting challenge mode

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

# I hope you enjoy my 21 Game. Have fun!
