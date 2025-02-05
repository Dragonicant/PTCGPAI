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
    cardCollection.displayCollection();  // Display all the cards

    return 0;
}