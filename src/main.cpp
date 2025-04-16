#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>
using namespace std;

//defining global objects/variables and functions to be accessed across the program
//misc functions, variables, objects etc
string name = ""; //user's name
string OSClearCommand = ""; //reused in console-clearing code if user specifies Windows or macOS/Linux
bool staying;   //whether or not the player/enemy/dealer wants to stay in their current turn
bool userStaying;   //whether or not the player wants to stay in their current turn
bool dealerStaying;   //whether or not the dealer wants to stay in their current turn
bool restart;   //whether or not the user wants to restart the game
bool instaWins = true;  //whether or not the user/enemy/dealer can win instantly (21 on first two cards)

//user instruction/prompt/input functions
int handleIntInput(int max) {
    int value;
    bool validInput = false;

    while (!validInput) {
        cout<<"\033[96m"<<"\n> ";
        if (cin >> value) {
            if (value > 0 && value <= max) {
                validInput = true;
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // bin the newline character
            } else {
                cout << "\033[35m"<<"Please enter a number between 1 and " << max << ".\n"<< "\033[0m";
            }
        } else {
            cout << "\033[35m"<<"Invalid input. Please enter a positive integer between 1 and " << max << " (inclusive)\n"<< "\033[0m";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    cout<<"\033[0m";
    return value;
}


void clrScr() {    //clears the console screen
    if(OSClearCommand.empty()) {
        cout<<"\033[2J\033[1;1H";
        cout<<"\033[2J\033[1;1H";
    } else {
        [[maybe_unused]] int clrResult=system(OSClearCommand.c_str());
        clrResult=system(OSClearCommand.c_str());
    }
}

void pressEnterToContinue() {   //simple cin function that waits for the user to press enter before ending the function
  this_thread::sleep_for(chrono::milliseconds(1200));
  string choice;
  cout<<"\nPRESS "<<"\033[32m"<<"ENTER"<<"\033[0m"<<" TO CONTINUE...\n";

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

void startingMessage() {  //prints a fancy opening logo/design thingy
    cout<<"\033[31m"<<"╔════════════════════════════════════╗\n";
    cout<<"║♥ ◆ ♠ ♣ ♥ ◆ ♠ ♣ |21| ♥ ◆ ♠ ♣ ♥ ◆ ♠ ♣║\n";
    cout<<"╚════════════════════════════════════╝\n\n"<<"\033[34m";

    cout<<"          _______     _____ \n";
    cout<<"         |   _   |   |__   |\n";
    cout<<"          \\_/ /  |      |  |\n";
    cout<<"             /  /       |  |\n";
    cout<<"            /  /        |  |\n";
    cout<<"           /  /         |  |\n";
    cout<<"          /  /          |  |\n";
    cout<<"         /  /____       |  |\n";
    cout<<"        |________|      |__|\n\n"<<"\033[31m";

    cout<<"╔════════════════════════════════════╗\n";
    cout<<"║     Draw, or Stay? Don't Bust!     ║\n";
    cout<<"╚════════════════════════════════════╝\n\n"<<"\033[0m";

    cout<<"Let's play a game of "<<"\033[34m"<<"21"<<"\033[0m"<<", "<<name<<"!\n";
}

bool promptReplay() {  //asks the user if they want to play again, exits the program if they do, returns true otherwise
    cout << "\nDo you want to play again, "<<name<<"?\n1. Yes\n2. No\n";
    int choice = handleIntInput(2);
    if(choice == 2) {
        cout << "\n\nThank you for playing my game, "<<name<<".";
        exit(0);
    }
    return true;
}

void promptInstaWin() {  //asks the user if they want to disable instant wins (player/dealer/enemy gets ace + 10 on their first draw of 2 cards)
    cout<<"\nSometimes, a player at the table/the dealer will obtain a hand total of 21 on their initial draw, causing an "<<"\033[1;33m"<<"instant win.\n"<<"\033[0m";
    cout<<"\nWould you like to disable the chance of an instant win (prevent ace + 10 combo being drawn at the start)?\n";
    cout<<"\n1. Yes\n2. No";

    if(handleIntInput(2)==1) {   //  ensure that no parties can insta win
        instaWins = false;
    }
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



//enemy player Struct, variables, objects, etc
struct Enemy {
    int id;
    int enemyHand[11];
    int openEnemySlot = 0;
    int enemyTotal = 0;
    bool enemStaying = false;
};

bool enemiesExist = false;   //whether or not the user wants to play with enemies
int enemiesCount;   //how many enemies the user indicated they want
vector<Enemy> enemies;  //vector of Enemy structs
int maxEnemTotal = 0;   //the highest total hand among all enemies

void promptEnemies() {   //asks the user if they want to play with enemies, and if so, how many
    clrScr();
    cout<<"\nDo you want to play with other players (computer)?\n1. Yes\n2. No\n";
    
    if(handleIntInput(2)==1) {
        enemiesExist = true;

        cout<<"\nHow many additional players do you want to play with? (Max: 6)\n";
        enemiesCount = handleIntInput(6);

        for(int i = 0; i < enemiesCount; i++) {
            enemies.push_back(Enemy{i, {}, 0, 0});
        }
        
        cout<<"\nYou will be playing with "<<enemiesCount<<" additional player(s).\n";
        this_thread::sleep_for(chrono::milliseconds(2500));
        clrScr();
    } else {
        cout<<"no problemo\n";
    }
}

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

    [[maybe_unused]] int cardsTaken = 0;
    for(bool b : taken) {
        if(b) {cardsTaken++;}
    }
    //cout<<"\nCards with 'taken' status: "<<cardsTaken<<"\n";
    
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

void printDecks(bool userStayed) {  //prints the user's, player 2's and dealer's hands, output varies based on whether or not the players have stayed
    clrScr();
    cout<<"\033[32m"<<"Your hand total: "<<userTotal<<"\nYour wins: "<<userWins<<"\n";
    cout<<"Your hand:\n";
    for(int i = 0; i < openUserSlot; i++) {
        cout<<"["<<mainDeck[userHand[i]].title<<"] ";
    }

    if(enemiesExist) {   //printing enemies' hands (if they exist)
        cout<<"\033[31m";
        for(int i = enemies.size() - 1; i >= 0; i--) {
            Enemy e = enemies[i];
            cout<<"\n\nPLAYER "<<(e.id + 1)<<"\nHand total: "<<e.enemyTotal;
            cout<<"\nHand:\n";
            for(int i = 0; i < e.openEnemySlot; i++) {
                cout<<"["<<mainDeck[e.enemyHand[i]].title<<"] ";
            }
        }
    }

    cout<<"\033[33m"<<"\n\nDealer's (visible) hand total: "<<dealerTotal<<"\nDealer's wins: "<<dealerWins;
    cout<<"\nDealer's hand:\n";
    if(!userStayed) {
        cout<<"["<<mainDeck[dealerHand[0]].title<<"] [?]";
    } else {
        for(int i = 0; i < openDealerSlot; i++) {
            cout<<"["<<mainDeck[dealerHand[i]].title<<"] ";
        }
    }
    cout<<"\033[0m"<<"\n\n";
    pressEnterToContinue();
    clrScr();
}

void intro() {  //opening sequence of the game, asks for user's name and whether or not they want to learn the rules
    cout << "Before we get started: What's your name?\n\n"<<"\033[34m"<<"> "<<"\033[96m";
    cin >> name;
    clrScr();
    cout << "\033[0m" << "Are you familiar with 21, " + name + "?\n1. Yes\n2. No\n";
    if (handleIntInput(2) == 2) {
        explain21();
        clrScr();
    } else {
        cout << "\nThen let's get started.\n";
        this_thread::sleep_for(chrono::milliseconds(1200));
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
    enemies.clear();
    enemiesExist = false;
    enemiesCount = 0;
    instaWins = true;
}

void initDeal() {  //deals the initial 2 cards each to the user, enemies and dealer
    userHand[openUserSlot] = dealCard(); openUserSlot++;
    userTotal+=cardValue(mainDeck[userHand[0]].value, userTotal);

    //edge case: if the user's first card is an ace/10, ensure the second card doesnt permit an instawin
    if(!instaWins && mainDeck[userHand[0]].value==10) {
        bool aceDrawn = false;

        //do-while loop so at least 1 draw is made
        do {
            userHand[openUserSlot] = dealCard();

            if(mainDeck[userHand[openUserSlot]].value==0) {
                aceDrawn = true;
                taken[userHand[openUserSlot]] = false;  //negating the card draw
            } else {
                aceDrawn = false;
                openUserSlot++;
                userTotal+=cardValue(mainDeck[userHand[1]].value, userTotal);
            }
        } while(aceDrawn);
    } else if (!instaWins && mainDeck[userHand[0]].value==0) {
        bool tenDrawn = false;

        //do-while loop so at least 1 draw is made
        do {
            userHand[openUserSlot] = dealCard(); 
            
            if(mainDeck[userHand[openUserSlot]].value==10) {
                tenDrawn = true;
                taken[userHand[openUserSlot]] = false;  //negating the card draw
            } else {
                tenDrawn = false;
                openUserSlot++;
                userTotal+=cardValue(mainDeck[userHand[1]].value, userTotal);
            }
        } while(tenDrawn);
    } else {  //proceed as normal
        userHand[openUserSlot] = dealCard(); openUserSlot++;
        userTotal+=cardValue(mainDeck[userHand[1]].value, userTotal);
    }
    
    if(userTotal==21) {
        cout<<"\nHOLY COW! BLACKJ-That's 21 (Hand total: "<<userTotal<<")! "<<"\033[32m"<<"You win!\n"<<"\033[0m";
        this_thread::sleep_for(chrono::milliseconds(2200));
        userWins++;
        restart = promptReplay();
        return;
    }

    if(enemiesExist) {   //dealing cards to enemies (if they exist)
        for (int i = 0; i < enemies.size(); ++i) {
            enemies[i].enemyHand[enemies[i].openEnemySlot] = dealCard();
            enemies[i].openEnemySlot++;
            enemies[i].enemyTotal += cardValue(mainDeck[enemies[i].enemyHand[0]].value, enemies[i].enemyTotal);

            //edge case: if the enemy's first card is an ace/10, ensure the second card doesnt permit an instawin (if instaWin disabled by user)
            if(!instaWins && mainDeck[enemies[i].enemyHand[0]].value==10) {
                bool aceDrawn = false;

                //do-while loop so at least 1 draw is made
                do {
                    enemies[i].enemyHand[enemies[i].openEnemySlot] = dealCard();
                    if(mainDeck[enemies[i].enemyHand[enemies[i].openEnemySlot]].value==0) {
                        aceDrawn = true;
                        //negating the card draw
                        taken[enemies[i].enemyHand[enemies[i].openEnemySlot]] = false;
                    } else {
                        aceDrawn = false;
                        enemies[i].openEnemySlot++;
                        enemies[i].enemyTotal+=cardValue(mainDeck[enemies[i].enemyHand[1]].value, enemies[i].enemyTotal);
                    }
                } while(aceDrawn);
            } else if (!instaWins && mainDeck[enemies[i].enemyHand[0]].value==0) {
                bool tenDrawn = false;

                //do-while loop so at least 1 draw is made
                do {
                    enemies[i].enemyHand[enemies[i].openEnemySlot] = dealCard(); 
                    if(mainDeck[enemies[i].enemyHand[enemies[i].openEnemySlot]].value==10) {
                        tenDrawn = true;
                        //negating the card draw
                        taken[enemies[i].enemyHand[enemies[i].openEnemySlot]] = false;
                    } else {
                        tenDrawn = false;
                        enemies[i].openEnemySlot++;
                        enemies[i].enemyTotal+=cardValue(mainDeck[enemies[i].enemyHand[1]].value, enemies[i].enemyTotal);
                    }
                } while(tenDrawn);
            } else {  //proceed as normal
                enemies[i].enemyHand[enemies[i].openEnemySlot] = dealCard();
                enemies[i].openEnemySlot++;
                enemies[i].enemyTotal += cardValue(mainDeck[enemies[i].enemyHand[1]].value, enemies[i].enemyTotal);
            }

            //if an enemy's first two cards are an Ace and a 10-value card, they automatically win
            if (enemies[i].enemyTotal == 21) {
                cout << "\nPlayer " << (i + 1) << " hit 21 (Hand total: "<<enemies[i].enemyTotal<<")! "<<"\033[31m"<<"You lose.\n"<<"\033[0m";
                this_thread::sleep_for(chrono::milliseconds(2200));
                restart = promptReplay();
                return;
            }
        }
    }

    dealerHand[openDealerSlot] = dealCard(); openDealerSlot++;
    dealerTotal+=cardValue(mainDeck[dealerHand[0]].value, dealerTotal);
    dealerHand[openDealerSlot] = dealCard(); openDealerSlot++;

    //edge case: if the dealer's first card is an ace/10, ensure the second card doesnt permit an instawin (if user disabled instawin possibility)
    if(!instaWins && mainDeck[dealerHand[0]].value==10) {
        bool aceDrawn = false;

        //do-while loop so at least 1 draw is made
        do {
            dealerHand[openDealerSlot] = dealCard(); 

            if(mainDeck[dealerHand[openDealerSlot]].value==0) {
                aceDrawn = true;
                taken[dealerHand[openDealerSlot]] = false;  //negating the card draw
            } else {
                aceDrawn = false;
                openDealerSlot++;
                dealerTotal+=cardValue(mainDeck[dealerHand[1]].value, dealerTotal);
            }
        } while(aceDrawn);
    } else if (!instaWins && mainDeck[dealerHand[0]].value==0) {
        bool tenDrawn = false;

        //do-while loop so at least 1 draw is made
        do {
            dealerHand[openDealerSlot] = dealCard(); 

            if(mainDeck[dealerHand[openDealerSlot]].value==0) {
                tenDrawn = true;
                taken[dealerHand[openDealerSlot]] = false;  //negating the card draw
            } else {
                tenDrawn = false;
                openDealerSlot++;
                dealerTotal+=cardValue(mainDeck[dealerHand[1]].value, dealerTotal);
            }
        } while(tenDrawn);
    } else {  //proceed as normal
        dealerHand[openDealerSlot] = dealCard(); openDealerSlot++;
        dealerTotal+=cardValue(mainDeck[dealerHand[1]].value, dealerTotal);
    }

    //edge case: if the dealer's first two cards are an Ace and a 10-value card, they automatically win
    if(dealerTotal==21) {
        cout<<"\nWelp. THAT just happened.\n";
        cout<<"\nDealer won (Hand total: "<<dealerTotal<<"). "<<"\033[31m"<<"You lose!"<<"\033[0m"<<"Sorry.\n";
        this_thread::sleep_for(chrono::milliseconds(2200));
        dealerWins++;
        restart = promptReplay();
        return;
    }
}

void userTurn() {   //code for the user's turn, runs until the user chooses to stay or goes bust
    while(!staying) {
          clrScr();
          //displaying the user's hand without displaying the dealer's face-down card
          printDecks(false);

          if(userTotal==21) {
              cout<<"\nHOLY COW! BLACKJ-That's 21! (Hand total: "<<userTotal<<") "<<"\033[32m"<<"You win!\n"<<"\033[0m";
              this_thread::sleep_for(chrono::milliseconds(2200));
              restart = promptReplay();
              userWins++;
              break; //break the subloop
          } else if(userTotal>21) {
              cout<<"\nWelp. THAT just happened.\n";
              cout<<"\033[31m"<<"\nYou went bust!"<<"\033[0m"<<" Sorry.\n";
              this_thread::sleep_for(chrono::milliseconds(2200));
              restart = promptReplay();
              dealerWins++;
              break; //break the subloop
          }
          
        //asking the user if they want to draw another card or stay
          cout<<"\nMake your choice, " + name + "!\n";
          cout<<"1. Draw another card\n2. Stay\n";

          if(handleIntInput(2)==1) {  //draw another card
              userHand[openUserSlot] = dealCard(); openUserSlot++;
              userTotal+=cardValue(mainDeck[userHand[openUserSlot-1]].value, userTotal);
          } else {
              staying = true;
              cout<<"\nYou have chosen to stay.\n"; pressEnterToContinue();
          }
      }
}

void enemTurn(int enemyID) { //code for the enemy's turn, runs until the enemy chooses to stay or goes bust
    int index = 0;  //index of the enemy in the enemies vector
    for(int i = 0; i < enemies.size(); i++) {
        if(enemies[i].id==enemyID) {
            index = i;
            break;
        }
    }

    Enemy &e = enemies[index];
    while(!staying) {
          clrScr();
          //displaying the hands without displaying the dealer's face-down card
          cout<<"Player "<<(enemyID + 1)<<"'s turn:\n";
          printDecks(false);
          if(e.enemyTotal==21) {
              cout<<"\nPlayer "<<(enemyID + 1)<<" hit 21! (Hand total: "<<e.enemyTotal<<") "<<"\033[31m"<<"You lose.\n"<<"\033[0m";
              this_thread::sleep_for(chrono::milliseconds(2200));
              restart = promptReplay();
              return; //exit the function
          } else if(e.enemyTotal>21) {
              cout<<"\nPlayer "<<(enemyID + 1)<<" went bust!\n";
              enemies.erase(enemies.begin() + index);  //remove the enemy from the game (delete its object from the vector)
              this_thread::sleep_for(chrono::milliseconds(2200));
              break; //break the subloop
          }


          //draw cards until the enemy's total is 17 or more
          if(e.enemyTotal<17) {
              e.enemyHand[e.openEnemySlot] = dealCard(); e.openEnemySlot++;
              e.enemyTotal+=cardValue(mainDeck[e.enemyHand[e.openEnemySlot-1]].value, e.enemyTotal);
          } else {  //if hit 17 or more, 50/50 chance to draw another card or stay
              if(rand() % 2 == 0) {
                  cout<<"Player "<<(enemyID + 1)<<" has chosen to draw another card.\n";
                  e.enemyHand[e.openEnemySlot] = dealCard(); e.openEnemySlot++;
                  e.enemyTotal+=cardValue(mainDeck[e.enemyHand[e.openEnemySlot-1]].value, e.enemyTotal);
              } else {
                  staying = true;
                  cout<<"Player "<<(enemyID + 1)<<" has chosen to stay.\n";
                  this_thread::sleep_for(chrono::milliseconds(1500));
              }
          }
      }
    staying = false;
}

void dealerTurn() {  //code for the dealer's turn, runs until the dealer chooses to stay or goes bust
    while(!staying) {
          clrScr();
          //displaying the dealer's full hand
          printDecks(true);

          if(dealerTotal<16) {
              cout<<"The dealer has chosen to draw another card.\n";
              dealerHand[openDealerSlot] = dealCard(); openDealerSlot++;
              dealerTotal+=cardValue(mainDeck[dealerHand[openDealerSlot-1]].value, dealerTotal);
          } else if(dealerTotal>21) {
              cout<<"\nWelp. THAT just happened.\n";
              cout<<"\nDealer went bust. "<<"\033[32m"<<"You win!\n"<<"\033[0m";
              this_thread::sleep_for(chrono::milliseconds(2200));
              restart = promptReplay();
              userWins++;
              break; //break the subloop
          } else if(dealerTotal==21) {
              cout<<"\nWelp. THAT just happened.\n";
              cout<<"\nDealer won, "<<"\033[31m"<<"you lose!"<<"\033[0m"<<" Sorry.\n";
              this_thread::sleep_for(chrono::milliseconds(2200));
              restart = promptReplay();
              dealerWins++;
              break; //break the subloop
          } else {
              cout<<"The dealer has chosen to stay.\n";
              staying = true;
              pressEnterToContinue();
              clrScr();
          }
      }
}

void determineWinner() {  //determines the winner of the round based on the user's and dealer's totals
    if(enemiesExist) {  //if enemies exist, account for them in the win-lose logic
        for(Enemy e : enemies) {
            if(e.enemyTotal>maxEnemTotal) {
                maxEnemTotal = e.enemyTotal;
            }
        }
    }
    
    if(userTotal>dealerTotal && userTotal>maxEnemTotal) {  //user wins
          cout<<"\nYour hand is worth more than the rest. "<<"\033[32m"<<"You win!\n"<<"\033[0m";
          this_thread::sleep_for(chrono::milliseconds(2200));
          restart = promptReplay();
          userWins++;
      } else if(userTotal==dealerTotal || userTotal==maxEnemTotal) {  //user ties with dealer/enemy
          cout<<"\nWelp. Seems like you and another party tied. "<<"\033[33m"<<"Nobody wins!\n"<<"\033[0m";
          this_thread::sleep_for(chrono::milliseconds(2200));
          restart = promptReplay();
      } else if(dealerTotal>maxEnemTotal) {  //dealer wins
          cout<<"\nThe dealer won. "<<"\033[31m"<<"You lose!\n"<<"\033[0m";
          this_thread::sleep_for(chrono::milliseconds(2200));
          dealerWins++;
          restart = promptReplay();
      } else if(dealerTotal>maxEnemTotal) {  //an enemy wins
          cout<<"\nAnother player won. "<<"\033[31m"<<"You lose!\n"<<"\033[0m";
          this_thread::sleep_for(chrono::milliseconds(2200));
          restart = promptReplay();
      } else {
          cout<<"\nAnother hand is worth more than yours. "<<"\033[31m"<<"You lose!\n"<<"\033[0m";
          this_thread::sleep_for(chrono::milliseconds(2200));
          restart = promptReplay();
      }
}

int main() {
      //defining variables/objects before starting the game
      int choice = 0; //variable for storing user integer input

      //preliminary configurations to improve user experience
      cout<<"This program implements code that clears the console periodically for aesthetic purposes.\nThis process might require knowledge of your operating system.";
      cout<<"\nSelect the operating system of the terminal/environment that you are running this program from:\n";
      cout<<"1. Windows\n2. macOS/Linux\n3. Other/not on command line\n";
      choice = handleIntInput(3);
      clrScr();
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
        
        clrScr();  //clearing console for aesthetic purposes
        startingMessage();
        pressEnterToContinue();

        //asking user if they would not want the possibility of any party getting a 21 within the first two draws
        promptInstaWin();

        //asking user if they want to play with enemies  
        promptEnemies();

      //dealing cards to the user, enemies and the dealer, and adding their values to their respective totals
      initDeal();

      //subloop for the user's turn, runs until the user chooses to stay or goes bust
      userTurn();

      if(restart) { //if an ending was reached and the user wants to restart, skip the rest of the loop
          continue;
      }

      //subloop for the enemies, runs until all enemies stay, bust or one wins. loop runs backwards to avoid skipping enemies (index adjustment stuff) when one is removed
      if(enemiesExist) {
          staying = false;
          for(int i = enemies.size() - 1; i >= 0; i--) {
              enemTurn(enemies[i].id);
          }
      }

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

      printDecks(true);

      //win-lose logic
      determineWinner();
      }
      return 0;
}
