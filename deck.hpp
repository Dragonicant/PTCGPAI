#ifndef DECK_HPP
#define DECK_HPP

#include "types.hpp"
#include <vector>
#include <memory>
#include <unordered_map>

using namespace std;

// Condensed Hand View Function
void displayCondensedHand(const vector<shared_ptr<Card>>& hand) {
    unordered_map<string, int> cardCount;

    // Count occurrences of each card in the hand
    for (const auto& card : hand) {
        cardCount[card->name]++;
    }

    cout << "Hand:" << endl;
    for (const auto& entry : cardCount) {
        cout << entry.second << "x " << entry.first << endl;  // Display the quantity and name of the card
    }
}

class Deck {
public:
    vector<shared_ptr<Card>> cards;  // A deck contains shared pointers to cards
    static const int MAX_DECK_SIZE = 20;  // Maximum number of cards in a deck
    static const int MAX_CARD_DUPLICATES = 2;  // Maximum duplicates of the same card

    // Function to add a card to the deck
    bool addCard(shared_ptr<Card> card) {
        if (cards.size() >= MAX_DECK_SIZE) {
            cout << "Cannot add card. Deck size limit of " << MAX_DECK_SIZE << " reached." << endl;
            return false;
        }

        // Check for duplicates
        int count = 0;
        for (const auto& existingCard : cards) {
            if (existingCard->name == card->name) {
                ++count;
            }
        }

        if (count >= MAX_CARD_DUPLICATES) {
            cout << "Cannot add more than " << MAX_CARD_DUPLICATES << " copies of the card: " << card->name << endl;
            return false;
        }

        cards.push_back(card);
        return true;
    }

    // Function to draw a card from the deck
    shared_ptr<Card> drawCard() {
        if (cards.empty()) {
            cout << "Deck is empty!" << endl;
            return nullptr;
        }

        shared_ptr<Card> drawnCard = cards.back();  // Draw the last card (this could be adjusted)
        cards.pop_back();  // Remove it from the deck
        return drawnCard;
    }

    // Function to make a shallow copy of the deck
    Deck shallowCopy() const {
        Deck copy;
        copy.cards = cards;  // Copy the vector of cards (shallow copy)
        return copy;
    }

    // Function to display the deck
    void displayDeck() const {
        cout << "Deck (" << cards.size() << " cards):" << endl;
        for (const auto& card : cards) {
            card->display();
            cout << "\n";  // Add a newline between cards for better separation
        }
        cout << endl;
    }

    // Function to verify the deck's validity
    bool verifyDeck() const {
        if (cards.size() > MAX_DECK_SIZE) {
            cout << "Deck exceeds the maximum size of " << MAX_DECK_SIZE << " cards!" << endl;
            return false;
        }

        unordered_map<string, int> cardCounts;
        for (const auto& card : cards) {
            cardCounts[card->name]++;
        }

        for (const auto& entry : cardCounts) {
            if (entry.second > MAX_CARD_DUPLICATES) {
                cout << "Card " << entry.first << " exceeds the maximum limit of " << MAX_CARD_DUPLICATES << " copies!" << endl;
                return false;
            }
        }

        return true;
    }

    // Function to display a condensed view of the deck (card name and quantity)
    void displayCondensedDeck() const {
        unordered_map<string, int> cardCounts;

        // Count occurrences of each card in the deck
        for (const auto& card : cards) {
            cardCounts[card->name]++;
        }

        // Display the condensed deck
        cout << "Condensed Deck View:" << endl;
        for (const auto& entry : cardCounts) {
            cout << entry.second << "x " << entry.first << endl;
        }
        cout << endl;
    }
};

// Renaming Deck to CardCollection for clarity
class CardCollection {
public:
    vector<Card> cards;  // Collection of cards

    // Add a card to the collection
    void addCard(const Card& card) {
        cards.push_back(card);
    }

    // Display all cards in the collection
    void displayCollection() const {
        for (const auto& card : cards) {
            card.display();
            cout << "\n"; // Add a newline between cards for better separation
        }
    }

    // Optional: method to find a card by name
    const Card* findCardByName(const string& cardName) const {
        for (const auto& card : cards) {
            if (card.name == cardName) {
                return &card;
            }
        }
        return nullptr;  // Return null if not found
    }

    // Optional: method to remove a card by name
    void removeCardByName(const string& cardName) {
        cards.erase(remove_if(cards.begin(), cards.end(),
            [&cardName](const Card& card) { return card.name == cardName; }),
            cards.end());
    }

    // Get total number of cards in the collection
    size_t getCardCount() const {
        return cards.size();
    }
};

#endif // DECK_HPP