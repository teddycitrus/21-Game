#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>
using namespace std;

//defining global objects/variables and functions to be accessed across the program
int handleIntInput(int max) {
    int value = 0;
    bool validInput = false;

    while (!validInput) {
        cout << "> "; // optional
        if (cin >> value) {
            if (value > 0 && value <= max) {
                validInput = true;
            } else {
                cout << "Please enter a number between 1 and " << max << ".\n";
            }
        } else {
            // invalid input
            cout << "Invalid input. Please enter a positive integer between 1 and "<<max<<"(inclusive)\n";
            cin.clear(); // clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // toss junk input
        }
    }

    return value;
}

void clrScr(string clearCode) {    //clears the console screen
    if(clearCode.empty()) {
        cout<<"\033[2J\033[1;1H";
    } else {
        [[maybe_unused]] int clrResult=system(clearCode.c_str());
    }
}

void pressEnterToContinue() {   //simple cin function that waits for the user to press enter before ending the function
  this_thread::sleep_for(chrono::milliseconds(2200));
  string choice;
  cout<<"PRESS ENTER TO CONTINUE...\n";
  getline(cin, choice);   //storing the input value in a string variable

  std::cout << "\033[F\033[2K\033[F\033[2K";
  //ANSI escape codes to clear the line that the user pressed enter on + the line that says 'press enter to continue'
  //literally does nothing with the input value, just waits for the user to press enter so that the main function can continue
}

void explain21() {    //explains the rules and process of the 21 Game, one line at a time
  cout<<"This program is a simple terminal-based '21 Card Game' that is\n";
  cout<<"DEFINITELY NOT reminiscent of a real-world casino game.\n\n";
    this_thread::sleep_for(chrono::milliseconds(2200));
    pressEnterToContinue(); 
  cout<<"The rules are simple: You and the other players (computer) are dealt two cards, face up. \n"; pressEnterToContinue();
  cout<<"The dealer (computer) receives one card face up and one card face down. \n"; pressEnterToContinue();
  cout<<"You and the other players will be given the option to repeatedly receive another card or 'stay'.\n"; pressEnterToContinue();
  cout<<"Opting to 'stay' renders you unable to receive additional cards for the rest of the current game.\n"; pressEnterToContinue();
  cout<<"Each card has a value corresponding to its face value.\n"; pressEnterToContinue();
  cout<<"However, the Jack, Queen and King are all worth 10, and the Ace is worth either 1 or 11.\n"; pressEnterToContinue();
  cout<<"If a player's hand totals >21, they 'bust' (lose).\n"; pressEnterToContinue();
  cout<<"The goal is to get as close to 21 as possible without going over.\n"; pressEnterToContinue();
  cout<<"Once all players have chosen to 'stay', or gone 'bust', the dealer reveals their face down card.\n"; pressEnterToContinue();
  cout<<"The dealer will then proceed to be dealt cards until their hand totals 17 or more, at which point they will 'stay'.\n"; pressEnterToContinue();
    cout<<"\033[2J\033[1;1H";
  cout<<"\nWin/Lose Conditions:\n"; pressEnterToContinue();
  cout<<"If a player goes 'bust' (hand total >21), they lose.\n";
    pressEnterToContinue();
  cout<<"If the dealer goes 'bust', all players who have not 'busted' win.\n"; pressEnterToContinue();
  cout<<"If the dealer does not go 'bust', the surviving player with the highest hand value wins.\n"; pressEnterToContinue();
  cout<<"If the dealer and a player have the same hand value, the highest in the round, it is a tie, and nobody wins.\n"; pressEnterToContinue();
  cout<<"That's all there is to it! Let's get started.\n"; pressEnterToContinue();
}

void startingMessage(string name) {
    cout<<"╔════════════════════════════════════╗\n";
    cout<<"║♥ ◆ ♠ ♣ ♥ ◆ ♠ ♣ |21| ♥ ◆ ♠ ♣ ♥ ◆ ♠ ♣║\n";
    cout<<"╚════════════════════════════════════╝\n\n";

    cout<<"          _______     _____ \n";
    cout<<"         |   _   |   |__   |\n";
    cout<<"          \\_/ /  |      |  |\n";
    cout<<"             /  /       |  |\n";
    cout<<"            /  /        |  |\n";
    cout<<"           /  /         |  |\n";
    cout<<"          /  /          |  |\n";
    cout<<"         /  /____       |  |\n";
    cout<<"        |________|      |__|\n\n";

    cout<<"╔════════════════════════════════════╗\n";
    cout<<"║     Draw, or Stay? Don't Bust!     ║\n";
    cout<<"╚════════════════════════════════════╝\n\n";

    cout<<"Let's play a game of 21, " + name + "!\n";
}

bool promptReplay(string name) {
    cout << "\nDo you want to play again, "<<name<<"?\n1. Yes\n2. No\n";
    int choice = handleIntInput(2);
    if(choice == 2) {
        cout << "\n\nThank you for playing my game, "<<name<<".";
        exit(0);
        return false;
    }
    return true;
}

//card struct, variables/arrays, and functions
struct Card {  //struct that stores the value and suit of a card
  int value;
  string title;
};

Card mainDeck[52];  //array of Card structs
bool taken[52] = {}; //whether or not a card has been taken

//hand size is 11, the max possible card count that can be drawn without busting
int userHand[11]; //stores the cards in the user's hand (indices of the mainDeck array)
int openUserSlot = 0; //the next open slot in the user's hand
int userTotal = 0; //the total value of the user's hand

int dealerHand[11]; //array of integers that stores the cards in the dealer's hand (indices of the mainDeck array)
int openDealerSlot = 0; //the next open slot in the dealer's hand
int dealerTotal = 0; //the total value of the dealer's hand
int userWins = 0; //total rounds won by user
int dealerWins = 0; //total rounds won by dealer

int dealCard() {   //'deals a card' by randomly selecting a number 0-51 corresponding to a mainDeck card that has not been taken yet
    srand(time(0));  //seeding the random number generator with the current time
    bool validNumber = false;  //boolean that stores whether or not the randomly generated number is valid (not taken yet)
    int randomNumber = 0;
    
    while(!validNumber) {  //loop that runs until a valid number is generated
        randomNumber = rand() % 52;  //generating a random number between 0 and 51
        
        if(!taken[randomNumber]) {  //if the card has not been taken yet
            validNumber = true;  //the number is valid
        }
    }

    taken[randomNumber] = true;   //mark card as taken
    return randomNumber;
}

int cardValue(int value, int total) {  //function that returns the value of a card, given its face value and the current total of the hand
    if(value!=0) {  //if the card is not an Ace
        return value;
    } else if(total>10) {  //if the card is an Ace and the total is greater than 10
        return 1;
    } else {
        return 11;
    }
}

void instantiateMainDeck() {  //function that instantiates the mainDeck array with all 52 cards
  //hearts
  for (int i = 0; i < 9; i++) {  //numbered cards
    mainDeck[i] = {i+2, to_string(i+2)+"♥"};
  }
  mainDeck[9] = {10, "J♥"}; mainDeck[10] = {10, "Q♥"}; mainDeck[11] = {10, "K♥"}; mainDeck[12] = {0, "A♥"};//Jack, Queen, King, Ace

  //diamonds
  for (int i = 13; i < 22; i++) {  //numbered cards
    mainDeck[i] = {i-11, to_string(i-11)+"◆"};
  }
  mainDeck[22] = {10, "J◆"}; mainDeck[23] = {10, "Q◆"}; mainDeck[24] = {10, "K◆"}; mainDeck[25] = {0, "A◆"};//Jack, Queen, King, Ace

  //spades
  for (int i = 26; i < 35; i++) {  //numbered cards
    mainDeck[i] = {i-24, to_string(i-24)+"♠"};
  }
  mainDeck[35] = {10, "J♠"}; mainDeck[36] = {10, "Q♠"}; mainDeck[37] = {10, "K♠"}; mainDeck[38] = {0, "A♠"};//Jack, Queen, King, Ace

  //clubs
  for (int i = 39; i < 48; i++) {  //numbered cards
    mainDeck[i] = {i-37, to_string(i-37)+"♣"};
  }
  mainDeck[48] = {10, "J♣"}; mainDeck[49] = {10, "Q♣"}; mainDeck[50] = {10, "K♣"}; mainDeck[51] = {0, "A♣"};//Jack, Queen, King, Ace
}

void printDecks1(string clear, bool userStayed) {  //prints the user's and dealer's hands, output varies based on whether or not the user has stayed
    clrScr(clear);
    cout<<"Your hand total: "<<userTotal<<"\nYour wins: "<<userWins<<"\n";
    cout<<"Your hand:\n";
    for(int i = 0; i < openUserSlot; i++) {
        cout<<mainDeck[userHand[i]].title + " ";
    }

    cout<<"\n\nDealer's hand total (visible cards): "<<dealerTotal<<"\nDealer's wins: "<<dealerWins;
    cout<<"\nDealer's hand (visible cards):\n";
    if(!userStayed) {
        cout<<mainDeck[dealerHand[0]].title;
    } else {
        for(int i = 0; i < openDealerSlot; i++) {
            cout<<mainDeck[dealerHand[i]].title + " ";
        }
    }
    cout<<"\n\n";
    pressEnterToContinue();
    clrScr(clear);
}

//other functions, variables, objects etc
string name = ""; //user's name
string OSClearCommand = ""; //reused in console-clearing code if user specifies Windows or macOS/Linux
bool staying;   //whether or not the player/dealer wants to stay
bool restart;   //whether or not the user wants to restart the game

void intro() {  //opening sequence of the game, asks for user's name and whether or not they want to learn the rules
    cout << "Before we get started: What's your name?\n\n> ";
    cin >> name;
    clrScr(OSClearCommand);
    cout << "Are you familiar with 21, " + name + "?\n1. Yes\n2. No\n";
    if (handleIntInput(2) == 2) {
        explain21();
        clrScr(OSClearCommand);
    } else {
        cout << "Then let's get started.\n";
        this_thread::sleep_for(chrono::milliseconds(2200));
    }
}

void resetStats() {  //reset the game state for the next round
    staying = false;
    restart = false;
    openUserSlot = 0; userTotal = 0;
    openDealerSlot = 0; dealerTotal = 0;
    for (int i = 0; i < 52; ++i) {
        taken[i] = false;
    }
}

void initDeal() {  //deals the initial 2 cards each to the user and dealer
    userHand[openUserSlot] = dealCard(); openUserSlot++;
    userTotal+=cardValue(mainDeck[userHand[openUserSlot-1]].value, userTotal);
    userHand[openUserSlot] = dealCard(); openUserSlot++;
    userTotal+=cardValue(mainDeck[userHand[openUserSlot-1]].value, userTotal);

    dealerHand[openDealerSlot] = dealCard(); openDealerSlot++;
    dealerTotal+=cardValue(mainDeck[dealerHand[0]].value, dealerTotal);
    dealerHand[openDealerSlot] = dealCard(); openDealerSlot++;
}

void userTurn() {   //code for the user's turn, runs until the user chooses to stay or goes bust
    while(!staying) {
          clrScr(OSClearCommand);
          //displaying the user's hand without displaying the dealer's face-down card
          printDecks1(OSClearCommand, false);

          if(userTotal==21) {
              cout<<"\nHOLY COW! BLACKJ-That's 21! You win!\n";
              this_thread::sleep_for(chrono::milliseconds(2200));
              restart = promptReplay(name);
              userWins++;
              break; //break the subloop
          } else if(userTotal>21) {
              cout<<"\nWelp. THAT just happened.\n";
              cout<<"\nYou went bust! Sorry.\n";
              this_thread::sleep_for(chrono::milliseconds(2200));
              restart = promptReplay(name);
              dealerWins++;
              break; //break the subloop
          }

          cout<<"Make your choice, " + name + "!\n";
          cout<<"1. Draw another card\n2. Stay\n";

          if(handleIntInput(2)==1) {  //draw another card
              userHand[openUserSlot] = dealCard(); openUserSlot++;
              userTotal+=cardValue(mainDeck[userHand[openUserSlot-1]].value, userTotal);
          } else {
              staying = true;
              cout<<"You have chosen to stay.\n"; pressEnterToContinue();
          }
      }
}

void dealerTurn() {  //code for the dealer's turn, runs until the dealer chooses to stay or goes bust
    while(!staying) {
          clrScr(OSClearCommand);
          //displaying the dealer's full hand
          printDecks1(OSClearCommand, true);

          if(dealerTotal<16) {
              cout<<"The dealer has chosen to draw another card.\n";
              dealerHand[openDealerSlot] = dealCard(); openDealerSlot++;
              dealerTotal+=cardValue(mainDeck[dealerHand[openDealerSlot-1]].value, dealerTotal);
          } else if(dealerTotal>21) {
              cout<<"\nWelp. THAT just happened.\n";
              cout<<"\nDealer went bust. You win!\n";
              this_thread::sleep_for(chrono::milliseconds(2200));
              restart = promptReplay(name);
              userWins++;
              break; //break the subloop
          } else if(dealerTotal==21) {
              cout<<"\nWelp. THAT just happened.\n";
              cout<<"\nDealer won, you lose! Sorry.\n";
              this_thread::sleep_for(chrono::milliseconds(2200));
              restart = promptReplay(name);
              dealerWins++;
              break; //break the subloop
          } else {
              cout<<"The dealer has chosen to stay.\n";
              staying = true;
              pressEnterToContinue();
              clrScr(OSClearCommand);
          }
      }
}

void determineWinner() {  //determines the winner of the round based on the user's and dealer's totals
    if(userTotal>dealerTotal) {  //user wins
          cout<<"\nYour hand is worth more than the dealer's. You win!\n";
          this_thread::sleep_for(chrono::milliseconds(2200));
          restart = promptReplay(name);
          userWins++;
      } else if(userTotal==dealerTotal) {
          cout<<"\nWelp. Seems like you two tied. Nobody wins!\n";
          this_thread::sleep_for(chrono::milliseconds(2200));
          restart = promptReplay(name);
      } else {
          cout<<"\nThe dealer's hand is worth more than yours. You lose!\n";
          this_thread::sleep_for(chrono::milliseconds(2200));
          restart = promptReplay(name);
          dealerWins++;
      }
}

int main() {
      //defining variables/objects before starting the game
      int choice = 0; //variable for storing user integer input

      //preliminary configurations to improve user experience
      cout<<"This program implements code aimed to clear the console periodically for aesthetic purposes. This process might require knowledge of your operating system.\n Select the operating system command line/terminal that you are executing this program from:\n"<<endl;
      cout<<"1. Windows\n2. macOS/Linux\n3. Other/not on command line\n\n\n";
      choice = handleIntInput(3);
      clrScr(OSClearCommand);
      //processing input (user OS)
      switch(choice) {
          case 1:  //program running on windows cmd
              OSClearCommand = "CLS";
          break;
          case 2:  //program running on macOS/Linux command line/terminal
              OSClearCommand = "clear";
          break;
          //if not on command line of any of those three OS (e.g. Replit), ANSI Escape codes will be attempted
              //not all IDE's will work with this method, however Replit is a working example
      }

      intro();
    
      instantiateMainDeck(); //instantiating the main deck of cards, it is the same for every game, and is not modified at any point
      this_thread::sleep_for(chrono::seconds(1));  //sleep for a second to 'delay' the program, make the loading feel more real

      while(true) {  //main loop that contains the actual 21 game. ends when the user wants to stop playing, effectively ending the game

        //resetting all variables and objects
        resetStats();
        
        clrScr(OSClearCommand);  //clearing console for aesthetic purposes
        startingMessage(name); this_thread::sleep_for(chrono::milliseconds(2200));
        pressEnterToContinue();

      //dealing cards to the user and the dealer, and adding their values to their respective totals
      initDeal();

      //subloop for the user's turn, runs until the user chooses to stay or goes bust
      userTurn();

      if(restart) { //if an ending was reached and the user wants to restart, skip the rest of the loop
          continue;
      }

      //dealer logic subloop, runs until the dealer chooses to stay or goes bust
      staying = false;
      dealerTotal+=cardValue(mainDeck[dealerHand[1]].value, dealerTotal);
      dealerTurn();

      if(restart) { //if an ending was reached and the user wants to restart, skip the rest of the loop
          continue;
      }

      printDecks1(OSClearCommand, true);

      //win-lose logic
      determineWinner();
      }
      return 0;
}
