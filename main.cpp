#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib> // For system()

#include "types.hpp"
#include "deck.hpp"
#include "utilities.hpp"

using namespace std;

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

    // Step 4: Create shallow copies of the decks for the battle
    Deck battleDeck1 = deck1.shallowCopy();
    Deck battleDeck2 = deck2.shallowCopy();

    // Step 5: Simulate a game where each deck draws a card into its hand
    cout << "Battle Start!" << endl;

    // Draw 3 cards from each deck as an example
    vector<shared_ptr<Card>> hand1;
    vector<shared_ptr<Card>> hand2;

    for (int i = 0; i < 3; ++i) {
        shared_ptr<Card> card1 = battleDeck1.drawCard();
        shared_ptr<Card> card2 = battleDeck2.drawCard();

        if (card1) hand1.push_back(card1);
        if (card2) hand2.push_back(card2);
    }

    // Step 6: Display the hands
    cout << "\nHand 1 (Condensed View):" << endl;
    displayCondensedHand(hand1);

    cout << "\nHand 2 (Condensed View):" << endl;
    displayCondensedHand(hand2);

    return 0;
}