#include <algorithm>
#include <cctype>
#include <compare>
#include <concepts>
#include <filesystem>
#include <iostream>
#include <iterator>
#include <string>
#include <string_view>
#include <span>
#include <vector>
#include <ranges>
#include <fstream>

auto blacks(std::string_view blacks) {
    return [blacks](std::string_view word) {
        return std::ranges::all_of(blacks, [word](auto letters) {
            return !word.contains(letters);
        });
    };
}

auto tips(std::span<std::string_view> tips) {
    return [tips](std::string_view word) {
        return std::ranges::all_of(tips, [word](std::string_view tip) {
            return std::ranges::all_of(std::ranges::zip_view(word, tip), [word](auto pair) {
                auto [letter, check] = pair;
                if (check >= 'A' && check <= 'Z') {
                    return std::toupper(letter) == std::toupper(check);
                } else if (check >= 'a' && check <= 'z') {
                    return word.contains(check) && check != letter;
                }
                return true;
            });
        });
    };       
}

auto yellows(std::string_view yellows) {
    return [yellows](std::string_view word) {
        return std::ranges::all_of(yellows, [word](auto letter) {
            return word.contains(letter);
        });
    };
}

auto greens(std::string_view letters) {
}

auto operator and (std::predicate<std::string_view> auto first, std::predicate<std::string_view> auto second)
{
    return [first, second](std::string_view word) {
        return first(word) && second(word);
    };
}

auto operator or (std::predicate<std::string_view> auto first, std::predicate<std::string_view> auto second)
{
    return [first, second](std::string_view word) {
        return first(word) || second(word);
    };
}

int main(int argc, const char *argv[])
{
    auto arguments = std::span{std::next(argv), static_cast<std::size_t>(argc -1)};

    auto black = std::string_view{};
    auto all_tips = std::vector<std::string_view>{};
    auto filename = std::filesystem::path{"/data/data/com.termux/files/home/Data/english/wordle.txt"};
    for(auto arg : arguments | std::views::transform([](auto &arg) { return std::string_view{arg}; })) 
    {
        if (arg.starts_with("-b")) {
            black = arg.substr(2);
        } else if (arg.starts_with("-T")) {
            all_tips.emplace_back(arg.substr(2));
        } else if (arg.starts_with("-")) {
            std::cout << "Usage : wordle [file] -b[black out letters] -T[tips see bellow]...\n\t all tips have '.' for a space that has nothing\n\t a lowercase letter for a yellow top\n\t uppercase letter for green tips\n\n";
            return 0;
        } else if (std::filesystem::is_regular_file(arg)) {
            filename = arg;
        } else {
            std::cout << arg << " file not found!\n\n";
            return 1;
        }
    }

    std::cout << 
        "black   : "<< black << "\n"
        "yellows : "; 
    std::ranges::copy(all_tips, std::ostream_iterator<std::string_view>(std::cout, ", "));
    std::cout << "\n";

    auto file = std::ifstream(filename);
    auto in = std::ranges::istream_view<std::string>(file);

    auto out = std::ostream_iterator<std::string>(std::cout, "\n");

    std::ranges::copy(in
        | std::views::filter(blacks(black)) 
        | std::views::filter(tips(all_tips))
        , out);
}
