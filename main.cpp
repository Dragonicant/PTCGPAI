#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib> // For system()
#include <algorithm>    
#include <random>

#include "types.hpp"
#include "deck.hpp"
#include "utilities.hpp"

using namespace std;

class Game {
public:
    Game(shared_ptr<Deck> player1Deck, shared_ptr<Deck> player2Deck) {
        // Create shallow copies of the player decks for use in the game state
        playerDecks[0] = player1Deck;
        playerDecks[1] = player2Deck;

        // Initialize GameState with shallow copies of the decks
        for (int i = 0; i < 2; i++) {
            for (const auto& card : playerDecks[i]->cards) {
                gameDecks[i].push_back(card);  // Copy each card into the game's deck
            }
        }

        // Randomly select who goes first
        random_device rd;
        default_random_engine rng(rd());
        currentPlayer = uniform_int_distribution<int>(0, 1)(rng);  // Randomly pick 0 or 1 for first player

        cout << "Player " << currentPlayer + 1 << " will go first!" << endl;

        // Draw 5 cards for each player
        drawInitialCards(0);  // Draw 5 cards for Player 1
        drawInitialCards(1);  // Draw 5 cards for Player 2
    }

    void shuffleDeck(int player) {
        shuffle(gameDecks[player].begin(), gameDecks[player].end(), default_random_engine(random_device()()));
        cout << "Player " << player + 1 << "'s deck has been shuffled.\n";
    }

    // Function to draw 5 cards for a player
    void drawInitialCards(int player) {
        for (int i = 0; i < 5; ++i) {
            shared_ptr<Card> drawnCard = drawCard(player);
            if (drawnCard) {
                playerHands[player].push_back(drawnCard);  // Add drawn card to player's hand
            }
        }

        cout << "Player " << player + 1 << " has drawn 5 cards." << endl;
    }

    // Draws a card from the deck and returns it
    shared_ptr<Card> drawCard(int player) {
        if (gameDecks[player].empty()) {
            cout << "Player " << player + 1 << "'s deck is empty.\n";
            return nullptr;
        }
        shared_ptr<Card> cardToDraw = gameDecks[player].back();
        gameDecks[player].pop_back();  // Remove the card from the deck
        return cardToDraw;
    }

    // Method to show each player's hand
    void showHands() const {
        for (int i = 0; i < 2; ++i) {
            cout << "Player " << i + 1 << " hand:" << endl;
            for (const auto& card : playerHands[i]) {
                cout << card->name << endl;  // Assuming Card has a name field
            }
            cout << endl;
        }
    }

private:
    shared_ptr<Deck> playerDecks[2];  // Physical decks for each player
    vector<shared_ptr<Card>> gameDecks[2];  // Game's working copies of the decks
    vector<shared_ptr<Card>> playerHands[2];  // Player hands

    int currentPlayer;  // Track which player's turn it is
};

int main() {
    //runScraper();
    CardCollection cardCollection;
    readCSVAndPopulateDeck("pokemon_cards.csv", cardCollection);  // Adjust the reading function accordingly
    //cardCollection.displayCollection();  // Display all the cards

    // Step 3: Create two Deck instances
    Deck deck1;
    Deck deck2;

    // Seed random number generator with the current time to ensure different results each time
    srand(static_cast<unsigned int>(time(0)));

    // Step 4: Add 6 random cards to the first deck
    for (int i = 0; i < 6; ++i) {
        // Randomly select an index from the cardCollection
        int randomIndex = rand() % cardCollection.cards.size();

        shared_ptr<Card> selectedCard = make_shared<Card>(cardCollection.cards[randomIndex]);

        // Try to add the randomly selected card to deck1
        if (!deck1.addCard(selectedCard)) {
            cout << "Failed to add card: " << selectedCard->name << endl;
        }
    }

    // Step 5: Add 6 random cards to the second deck
    for (int i = 0; i < 6; ++i) {
        // Randomly select an index from the cardCollection
        int randomIndex = rand() % cardCollection.cards.size();

        shared_ptr<Card> selectedCard = make_shared<Card>(cardCollection.cards[randomIndex]);

        // Try to add the randomly selected card to deck2
        if (!deck2.addCard(selectedCard)) {
            cout << "Failed to add card: " << selectedCard->name << endl;
        }
    }

    // Assuming deck1 and deck2 are Deck objects
    shared_ptr<Deck> deck1Ptr = make_shared<Deck>(deck1);
    shared_ptr<Deck> deck2Ptr = make_shared<Deck>(deck2);

    // Instantiate a new game with the player decks
    Game game(deck1Ptr, deck2Ptr);

    // Display hands for both players
    game.showHands();

    return 0;
}