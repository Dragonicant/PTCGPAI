#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib> // For system()

#include "types.hpp"
#include "stages.hpp"

using namespace std;

// Function to safely convert string to integer with error checking
int safeStoi(const string& str, int defaultValue = 0) {
    try {
        return stoi(str);
    }
    catch (const std::invalid_argument& e) {
        cout << "Invalid argument for stoi: " << str << ", using default value: " << defaultValue << endl;
        return defaultValue; // Return default value in case of error
    }
    catch (const std::out_of_range& e) {
        cout << "Out of range error for stoi: " << str << ", using default value: " << defaultValue << endl;
        return defaultValue; // Return default value in case of error
    }
}

struct EnergyRequirement {
    char type; // Energy type (e.g., "F", "G", etc.)
    int amount;  // Number of energy required

    // Convert the energy requirement to a string in the form of "GXX" or "FF"
    string toEnergyString() const {
        return string(amount, type);  // Repeat the energy type 'amount' times
    }

    void display() const {
        cout << amount << " " << PTCGPTypes::getTypeName(type);
    }
};

struct Attack {
    string name;  // Name of the attack
    int damage;   // Damage dealt by the attack
    vector<EnergyRequirement> energyRequirement;  // Energy required to use the attack
    int effectId; // ID of the effect (use -1 for no effect)

    // Convert the energy requirements to a string like "GXX", "FF"
    string toEnergyString() const {
        string energyStr;
        for (const auto& req : energyRequirement) {
            energyStr += req.toEnergyString();  // Combine all energy types for this attack
        }
        return energyStr;
    }

    void display() const {
        cout << "    - " << name << " (Damage: " << damage << ", Energy Required: ";
        string energyStr = toEnergyString();  // Get the energy string for display
        cout << energyStr;
        if (effectId != -1) {
            cout << ", Effect ID: " << effectId;
        }
        cout << ")\n";
    }
};

// Basic card structure
class Card {
public:
    string name;
    int cardID;
    int hp;
    char type; // Type as a char
    int stage;
    vector<Attack> attacks;
    int abilID;
    char weakness; // Weakness as a char now
    int retreatCost;

    // Updated constructor to accept char for type and weakness
    Card(string n, int id, int health, char t, int s, vector<Attack> atk, int abilID, char weak, int retreat)
        : name(n), cardID(id), hp(health), type(t), stage(s), attacks(atk), abilID(abilID), weakness(weak), retreatCost(retreat) {}

    // Updated display method to use PTCGPTypes::getTypeName for type and weakness
    void display() const {
        cout << name << "\n";
        cout << "HP: " << hp << " | Type: " << type << " | Stage: " << stage << "\n";
        cout << "Weakness: " << weakness << " | Retreat Cost: " << retreatCost << "\n";

        for (const auto& attack : attacks) {
            string energyStr = attack.toEnergyString();  // Get energy string for the attack

            cout << left << setw(6) << energyStr   // Left-align, width 6
                << setw(20) << attack.name.substr(0, 20)  // Left-align, truncate to 20 chars
                << right << setw(4) << attack.damage  // Right-align, width 4
                << "\n";
        }
        cout << endl;
    }
};

// Player structure
class Player {
public:
    std::string name;
    std::vector<std::shared_ptr<Card>> deck;

    Player(std::string n) : name(n) {}
    void addCardToDeck(std::shared_ptr<Card> card) {
        deck.push_back(card);
    }
    void showDeck() const {
        std::cout << name << "'s Deck:\n";
        for (const auto& card : deck) {
            card->display();
            cout << "\n";  // Add a newline between cards for better separation
        }
    }
};

// Basic game loop
void gameLoop(Player& p1, Player& p2) {
    std::cout << "Starting game between " << p1.name << " and " << p2.name << "!\n";
    p1.showDeck();
    p2.showDeck();

    // Placeholder for game logic
    std::cout << "Game simulation not implemented yet.\n";
}

// Function to run the Python script and get URLs
void runScraper() {
    std::string pythonScript = "python scraper.py";  // Path to your Python script
    std::cout << "Running Python Scraper...\n";
    int result = system(pythonScript.c_str());
    if (result != 0) {
        std::cout << "Error running the Python script.\n";
    }
}

// Deck class
class Deck {
public:
    vector<Card> cards;

    void addCard(const Card& card) {
        cards.push_back(card);
    }

    void displayDeck() const {
        for (const auto& card : cards) {
            card.display();
            cout << "\n"; // Add a newline between cards for better separation
        }
    }
};

// Helper function to parse energy cost string into vector of EnergyRequirement
vector<EnergyRequirement> parseEnergyCost(const string& energyCostStr) {
    vector<EnergyRequirement> energyRequirements;

    if (energyCostStr.empty()) {
        return energyRequirements;  // Return empty if no energy
    }

    char currentType = energyCostStr[0];  // Start with the first energy type
    int count = 1;  // Start counting the energy

    // Loop through the string starting from the second character
    for (size_t i = 1; i < energyCostStr.size(); ++i) {
        if (energyCostStr[i] == currentType) {
            ++count;  // If the same type, increase the count
        }
        else {
            // If a different energy type is encountered, store the previous one
            energyRequirements.push_back({ currentType, count });
            currentType = energyCostStr[i];  // Update to the new type
            count = 1;  // Reset count for the new type
        }
    }

    // Don't forget to add the last one
    energyRequirements.push_back({ currentType, count });

    return energyRequirements;
}

// Read CSV and populate deck
void readCSVAndPopulateDeck(const string& filename, Deck& deck) {
    ifstream file(filename);
    string line;

    // Skip the header line
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string cardName, hpStr, typeStr, stageStr, weaknessStr, retreatCostStr;
        string attackName1, attackDamage1, energyCosts1;
        string attackName2, attackDamage2, energyCosts2;

        // Parse the CSV line
        getline(ss, cardName, ',');
        getline(ss, hpStr, ',');
        getline(ss, typeStr, ',');
        getline(ss, stageStr, ',');
        getline(ss, weaknessStr, ',');
        getline(ss, retreatCostStr, ',');
        getline(ss, attackName1, ',');
        getline(ss, attackDamage1, ',');
        getline(ss, energyCosts1, ',');
        getline(ss, attackName2, ',');
        getline(ss, attackDamage2, ',');
        getline(ss, energyCosts2, ',');

        // Convert string data into appropriate types with error handling
        int hp = safeStoi(hpStr);
        char type = typeStr[0];  // Just use the first character (G, F, etc.)
        int stage = safeStoi(stageStr);
        char weakness = weaknessStr[0];  // Just use the first character (G, F, etc.)
        int retreatCost = safeStoi(retreatCostStr);

        // Create attacks (up to 2 attacks per card)
        vector<Attack> attacks;

        Attack attack1 = {
            attackName1,
            safeStoi(attackDamage1),
            parseEnergyCost(energyCosts1)  // Parse the energy cost string into requirements
        };
        attacks.push_back(attack1);

        if (!attackName2.empty()) {
            Attack attack2 = {
                attackName2,
                safeStoi(attackDamage2),
                parseEnergyCost(energyCosts2)  // Parse the energy cost string into requirements
            };
            attacks.push_back(attack2);
        }

        // Create the card object
        Card card(cardName, 0, hp, type, stage, attacks, 0, weakness, retreatCost);

        // Add the card to the deck
        deck.addCard(card);
    }
}

int main() {
    //runScraper();
    Deck deck;
    readCSVAndPopulateDeck("pokemon_cards.csv", deck);
    deck.displayDeck();

    return 0;
}