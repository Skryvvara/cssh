#include <unistd.h>
#include <ncurses.h>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#define SEPARATOR std::filesystem::path::preferred_separator

struct Entry {
    std::string name;
    std::string hostname;
    std::string user;
};

WINDOW* draw_menu(WINDOW* win, int start_y, int start_x, int highlight, std::vector<Entry> entries) {
    int x = 1, y = 1; // Offset to leave space for window border

    // Clear the window
    werase(win);

    box(win, 0, 0);

    // Print the header row
    mvwprintw(win, y, x, "Host");
    mvwprintw(win, y, x + 22, "Hostname");
    mvwprintw(win, y, x + 47, "User");

    y++; // Move to the next line for data rows

    // Print data rows
    for (size_t i = 0; i < entries.size(); ++i) {
        if (static_cast<int>(i) == highlight) {
            wattron(win, A_REVERSE); // Highlight the selected row
            mvwprintw(win, y, x, entries[i].name.c_str());
            mvwprintw(win, y, x + 22, entries[i].hostname.c_str());
            mvwprintw(win, y, x + 47, entries[i].user.c_str());
            wattroff(win, A_REVERSE); // Turn off the highlight
        } else {
            mvwprintw(win, y, x, entries[i].name.c_str());
            mvwprintw(win, y, x + 22, entries[i].hostname.c_str());
            mvwprintw(win, y, x + 47, entries[i].user.c_str());
        }
        ++y;
    }

    wrefresh(win); // Refresh the window

    return win;
}

std::string join_path(const std::vector<std::string>& args) {
    std::string path = "";
    for (auto i = args.begin(); i != args.end(); i++) {
        path += *i;
        if (std::next(i) != args.end()) {
            path += SEPARATOR;
        }
    }
    return path;
}

std::vector<Entry> parse_ssh_config() {
    std::string home = std::getenv("HOME");

    std::string file = join_path({home, ".ssh", "config"});

    std::ifstream infile(file);

    std::vector<Entry> entries;
    Entry currentEntry;
    bool inHostBlock = false;

    std::string line;
    while (std::getline(infile, line))
    {
        // Remove leading and trailing whitespace
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        if (line.empty() || line[0] == '#') {
            continue; // Skip empty lines and comments
        }

        std::istringstream iss(line);
        std::string keyword;
        iss >> keyword;

        std::transform(keyword.begin(), keyword.end(), keyword.begin(), [](unsigned char c){ return std::tolower(c); });

        if (keyword == "host") {
            if (inHostBlock) {
                // Save the current entry before starting a new one
                entries.push_back(currentEntry);
            }
            inHostBlock = true;
            iss >> currentEntry.name;
            currentEntry.hostname.clear();
            currentEntry.user.clear();
        } else if (keyword == "hostname") {
            iss >> currentEntry.hostname;
        } else if (keyword == "user") {
            iss >> currentEntry.user;
        }
    }

    if (inHostBlock) {
        // Save the last entry
        entries.push_back(currentEntry);
    }

    return entries;
}

int main() {
    while (true) {
        initscr(); // Initialize ncurses
        cbreak(); // Line buffering disabled
        noecho(); // Don't echo while we do getch
        keypad(stdscr, TRUE); // Enable function keys

        std::vector<Entry> entries = parse_ssh_config();

        int highlight = 0;
        int choice = -1; // -1 means no choice made yet
        int c;

        // Calculate center coordinates for the menu window
        int max_y, max_x;
        getmaxyx(stdscr, max_y, max_x); // Get terminal window size

        int table_width = 75; // Width of the table
        int table_height = entries.size() + 3; // Height of the table including header and border

        int start_x = (max_x - table_width) / 2; // Calculate starting x-coordinate
        int start_y = (max_y - table_height) / 2; // Calculate starting y-coordinate

        // Create a window for the menu
        WINDOW* menu_win = newwin(table_height, table_width, start_y, start_x);
        box(menu_win, 0, 0); // Draw a box around the window
        refresh();

        while (true) {
            WINDOW* table = draw_menu(menu_win, start_y, start_x, highlight, entries);

            c = getch(); // Get user input
            switch (c) {
                case KEY_UP:
                    highlight = (highlight - 1 + entries.size()) % entries.size();
                    break;
                case KEY_DOWN:
                    highlight = (highlight + 1) % entries.size();
                    break;
                case 10: // Enter key
                    choice = highlight;
                    break;
            }
            if (c == 10) // User pressed Enter, break loop
                break;
        }

        // Print the selected row
        if (choice != -1) {
            // Clear previous message area
            werase(stdscr);
            endwin(); // End ncurses

            // Perform your system call, e.g., SSH

            Entry selected_entry = entries[choice];

            std::string cmd = "ssh " + selected_entry.user + "@" + selected_entry.hostname;

            int ret = system(cmd.c_str());
            const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
            write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
        }

        refresh();
        endwin(); // End ncurses
    }

    return 0;
}
