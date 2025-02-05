#ifndef TYPES_HPP
#define TYPES_HPP

#include <unordered_map>
#include <string>
#include <stdexcept>

class PTCGPTypes {
public:
    // Method to get the full type name from one-character abbreviation
    static std::string getTypeName(char abbreviation) {
        // Mapping of one-character abbreviations to full Pokémon type names
        static const std::unordered_map<char, std::string> abbreviationToType = {
            {'G', "Grass"},
            {'F', "Fire"},
            {'W', "Water"},
            {'L', "Lightning"},
            {'P', "Psychic"},
            {'I', "Fighting"},
            {'D', "Darkness"},
            {'M', "Metal"},
            {'N', "Normal"},
            {'R', "Dragon"}
        };

        // Check if the abbreviation exists in the map and return the full type name
        auto it = abbreviationToType.find(abbreviation);
        if (it != abbreviationToType.end()) {
            return it->second;
        }
        else {
            throw std::invalid_argument("Invalid abbreviation: " + std::string(1, abbreviation));
        }
    }
};

#endif // TYPES_HPP