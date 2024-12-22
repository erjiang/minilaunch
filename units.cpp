#include "units.h"
#include <string>
#include <regex>
#include <array>
#include <memory>
#include <iostream>
#include <sstream>
#include <optional>

bool isUnitsConversion(const std::string& text) {
    // Check for " in " substring (case insensitive)
    auto pos = std::search(
        text.begin(), text.end(),
        " in ", " in " + 4,
        [](char c1, char c2) { return std::tolower(c1) == std::tolower(c2); }
    );
    if (pos == text.end()) {
        return false;
    }

    // Get the left part before " in "
    std::string leftPart = text.substr(0, pos - text.begin());

    // Trim whitespace
    leftPart.erase(0, leftPart.find_first_not_of(" \t"));
    leftPart.erase(leftPart.find_last_not_of(" \t") + 1);

    // Match pattern: optional sign, numbers (with optional decimal), followed by letters
    std::regex numberPattern("^\\s*[-+]?\\d*\\.?\\d+\\s*[a-zA-Z]+$");
    return std::regex_match(leftPart, numberPattern);
}

std::optional<double> convertUnits(const std::string& fromUnit, const std::string& toUnit) {
    std::array<char, 128> buffer;
    std::string result;

    // Create the command string
    std::string cmd = "units \"" + fromUnit + "\" \"" + toUnit + "\"";

    // Open pipe to command
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        std::cerr << "Failed to run units command" << std::endl;
        return std::nullopt;
    }

    // Read the output
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    // Get first line
    std::string firstLine = result.substr(0, result.find('\n'));

    // Check if conversion was successful
    if (firstLine.empty() || firstLine.find('*') == std::string::npos) {
        std::cerr << "Unexpected output from units command: " << firstLine << std::endl;
        return std::nullopt;
    }

    // Parse the number (skip whitespace and *)
    try {
        size_t startPos = firstLine.find('*') + 1;
        return std::stod(firstLine.substr(startPos));
    } catch (const std::exception& e) {
        std::cerr << "Failed to parse units conversion result: " << e.what() << std::endl;
        return std::nullopt;
    }
}
