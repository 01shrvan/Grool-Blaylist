#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <random>
#include <algorithm>
#include <chrono>
#include <thread>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <sstream>

// ANSI color codes for console output
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"

class Song {
public:
    std::string title;
    std::string artist;
    std::vector<std::string> moods;
    int energy;
    int danceability;
    int year;
    int playCount;

    Song() : title(""), artist(""), energy(0), danceability(0), year(0), playCount(0) {}

    Song(std::string t, std::string a, std::vector<std::string> m, int e, int d, int y)
        : title(t), artist(a), moods(m), energy(e), danceability(d), year(y), playCount(0) {}

    bool operator==(const Song& other) const {
        return title == other.title && artist == other.artist;
    }
};

class MoodPlaylistGenerator {
private:
    std::vector<Song> songDatabase;
    std::vector<std::string> moodOptions;
    std::map<std::string, std::vector<Song>> userFavorites;
    int userHappinessLevel;
    std::map<std::string, int> moodCounts;

    void initializeSongDatabase() {
        songDatabase = {
            Song("Happy", "Pharrell Williams", {"happy", "energetic"}, 8, 7, 2013),
            Song("Someone Like You", "Adele", {"sad", "emotional"}, 4, 2, 2011),
            Song("Thunderstruck", "AC/DC", {"energetic", "powerful"}, 9, 6, 1990),
            Song("Relaxing Piano", "John Smith", {"calm", "relaxed"}, 2, 1, 2020),
            Song("Party Rock Anthem", "LMFAO", {"party", "energetic"}, 9, 9, 2011),
            Song("The Scientist", "Coldplay", {"melancholy", "thoughtful"}, 3, 2, 2002),
            Song("Don't Stop Believin'", "Journey", {"motivational", "uplifting"}, 7, 6, 1981),
            Song("Bohemian Rhapsody", "Queen", {"epic", "emotional"}, 6, 4, 1975),
            Song("Smooth Jazz Compilation", "Various Artists", {"relaxed", "calm"}, 3, 2, 2019),
            Song("Eye of the Tiger", "Survivor", {"motivational", "energetic"}, 8, 7, 1982),
            Song("Imagine", "John Lennon", {"thoughtful", "calm"}, 5, 3, 1971),
            Song("Dancing Queen", "ABBA", {"happy", "party"}, 7, 8, 1976),
            Song("Stairway to Heaven", "Led Zeppelin", {"epic", "thoughtful"}, 6, 4, 1971),
            Song("Smells Like Teen Spirit", "Nirvana", {"energetic", "powerful"}, 8, 6, 1991),
            Song("Wonderwall", "Oasis", {"melancholy", "uplifting"}, 5, 4, 1995)
        };

        moodOptions = {"happy", "sad", "energetic", "calm", "party", "melancholy", "motivational", "epic", "relaxed", "thoughtful"};
    }

    void displayHeader(const std::string& title) {
        std::cout << MAGENTA << "\n╔══════════════════════════════════════════════════════╗\n"
                  << "║ " << std::setw(50) << std::left << title << "║\n"
                  << "╚══════════════════════════════════════════════════════╝\n" << RESET;
    }

    void slowPrint(const std::string& text, int delay = 30) {
        for (char c : text) {
            std::cout << c << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        }
        std::cout << std::endl;
    }

    std::string getUserMood() {
        std::cout << CYAN << "How are you feeling today? Choose a mood:\n" << RESET;
        for (size_t i = 0; i < moodOptions.size(); ++i) {
            std::cout << i + 1 << ". " << moodOptions[i] << "\n";
        }

        int choice;
        while (true) {
            std::cout << YELLOW << "Enter your choice (1-" << moodOptions.size() << "): " << RESET;
            std::cin >> choice;
            if (choice > 0 && choice <= static_cast<int>(moodOptions.size())) {
                moodCounts[moodOptions[choice - 1]]++;
                return moodOptions[choice - 1];
            }
            std::cout << RED << "Invalid choice. Please try again.\n" << RESET;
        }
    }

    std::vector<Song> generatePlaylist(const std::string& mood, int playlistSize) {
        std::vector<Song> matchingSongs;
        for (const auto& song : songDatabase) {
            if (std::find(song.moods.begin(), song.moods.end(), mood) != song.moods.end()) {
                matchingSongs.push_back(song);
            }
        }

        // Add user favorites that match the mood
        for (const auto& favorite : userFavorites[mood]) {
            if (std::find(matchingSongs.begin(), matchingSongs.end(), favorite) == matchingSongs.end()) {
                matchingSongs.push_back(favorite);
            }
        }

        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(matchingSongs.begin(), matchingSongs.end(), g);

        // Sort by energy level for more coherent playlist flow
        std::sort(matchingSongs.begin(), matchingSongs.end(), [](const Song& a, const Song& b) {
            return a.energy < b.energy;
        });

        if (matchingSongs.size() > static_cast<size_t>(playlistSize)) {
            matchingSongs.resize(playlistSize);
        }

        // Increment play count for selected songs
        for (auto& song : matchingSongs) {
            song.playCount++;
        }

        return matchingSongs;
    }

    void displayPlaylist(const std::vector<Song>& playlist) {
        std::cout << GREEN << "\nYour AI-generated playlist:\n" << RESET;
        for (size_t i = 0; i < playlist.size(); ++i) {
            std::cout << CYAN << i + 1 << ". " << playlist[i].title << " - " << playlist[i].artist << " (" << playlist[i].year << ")" << RESET;
            std::cout << " [Energy: " << std::string(playlist[i].energy, '|') 
                      << ", Danceability: " << std::string(playlist[i].danceability, '|') 
                      << ", Plays: " << playlist[i].playCount << "]\n";
        }
    }

    void simulateAIProcessing() {
        std::vector<std::string> steps = {
            "Analyzing your mood...",
            "Scanning music database...",
            "Applying advanced AI algorithms...",
            "Optimizing song selection based on user preferences...",
            "Considering song energy levels for smooth transitions...",
            "Analyzing play history for personalized recommendations...",
            "Finalizing personalized playlist..."
        };

        for (const auto& step : steps) {
            slowPrint(YELLOW + step + RESET);
            std::this_thread::sleep_for(std::chrono::milliseconds(800));
        }
    }

    void displayMoodAnalysis(const std::string& mood) {
        std::cout << BLUE << "\nMood Analysis:\n" << RESET;
        std::cout << "Your current mood: " << mood << "\n";
        std::cout << "Happiness level: [";
        for (int i = 0; i < 10; ++i) {
            if (i < userHappinessLevel) {
                std::cout << "#";
            } else {
                std::cout << "-";
            }
        }
        std::cout << "] (" << userHappinessLevel << "/10)\n";

        // Display mood history
        std::cout << "\nYour mood history:\n";
        for (const auto& pair : moodCounts) {
            std::cout << pair.first << ": " << std::string(pair.second, '*') << "\n";
        }
    }

    void provideMoodRecommendation(const std::string& mood) {
        std::cout << GREEN << "\nMood Recommendation:\n" << RESET;
        if (mood == "sad" || mood == "melancholy") {
            std::cout << "Consider engaging in activities you enjoy or reaching out to friends for support.\n";
        } else if (mood == "happy" || mood == "energetic") {
            std::cout << "Channel your positive energy into a creative project or share your enthusiasm with others.\n";
        } else if (mood == "calm" || mood == "relaxed") {
            std::cout << "This is an ideal time for meditation, journaling, or focusing on personal growth.\n";
        } else {
            std::cout << "Embrace your current mood and use it as inspiration for your day's activities.\n";
        }
    }

    void displayAsciiArt() {
        std::cout << CYAN << R"(
   _____                 _   ____  _             _ _     _   
  / ____|               | | |  _ \| |           | (_)   | |  
 | |  __ _ __ ___   ___ | | | |_) | | __ _ _   _| |_ ___| |_ 
 | | |_ | '__/ _ \ / _ \| | |  _ <| |/ _` | | | | | / __| __|
 | |__| | | | (_) | (_) | | | |_) | | (_| | |_| | | \__ \ |_ 
  \_____|_|  \___/ \___/|_| |____/|_|\__,_|\__, |_|_|___/\__|
                                            __/ |            
                                           |___/             
)" << RESET;
    }

    void saveUserPreferences() {
        std::ofstream file("user_preferences.txt");
        if (file.is_open()) {
            file << userHappinessLevel << "\n";
            for (const auto& pair : userFavorites) {
                file << pair.first << "\n";
                for (const auto& song : pair.second) {
                    file << song.title << "," << song.artist << "\n";
                }
                file << "END_MOOD\n";
            }
            for (const auto& pair : moodCounts) {
                file << pair.first << "," << pair.second << "\n";
            }
            file.close();
        }
    }

    void loadUserPreferences() {
        std::ifstream file("user_preferences.txt");
        if (file.is_open()) {
            file >> userHappinessLevel;
            std::string line;
            std::getline(file, line); // Consume newline
            std::string currentMood;
            while (std::getline(file, line)) {
                if (line == "END_MOOD") {
                    currentMood = "";
                } else if (currentMood.empty() && line.find(',') == std::string::npos) {
                    currentMood = line;
                } else if (line.find(',') != std::string::npos) {
                    size_t commaPos = line.find(',');
                    std::string mood = line.substr(0, commaPos);
                    int count = std::stoi(line.substr(commaPos + 1));
                    moodCounts[mood] = count;
                } else {
                    size_t commaPos = line.find(',');
                    if (commaPos != std::string::npos) {
                        std::string title = line.substr(0, commaPos);
                        std::string artist = line.substr(commaPos + 1);
                        for (const auto& song : songDatabase) {
                            if (song.title == title && song.artist == artist) {
                                userFavorites[currentMood].push_back(song);
                                break;
                            }
                        }
                    }
                }
            }
            file.close();
        }
    }

    void addToFavorites(const Song& song, const std::string& mood) {
        userFavorites[mood].push_back(song);
        std::cout << GREEN << "Added '" << song.title << "' to your favorites for " << mood << " mood.\n" << RESET;
    }

    void displayFavorites() {
        std::cout << BLUE << "\nYour Favorite Songs:\n" << RESET;
        for (const auto& pair : userFavorites) {
            std::cout << CYAN << pair.first << " mood:\n" << RESET;
            for (const auto& song : pair.second) {
                std::cout << "  - " << song.title << " by " << song.artist << "\n";
            }
        }
    }

    void displayMostPlayedSongs() {
        std::vector<Song> sortedSongs = songDatabase;
        std::sort(sortedSongs.begin(), sortedSongs.end(), [](const Song& a, const Song& b) {
            return a.playCount > b.playCount;
        });

        std::cout << BLUE << "\nYour Most Played Songs:\n" << RESET;
        for (size_t i = 0; i < 5 && i < sortedSongs.size(); ++i) {
            std::cout << CYAN << i + 1 << ". " << sortedSongs[i].title << " - " << sortedSongs[i].artist 
                      << " (Plays: " << sortedSongs[i].playCount << ")\n" << RESET;
        }
    }

    void displayMoodInsights() {
        std::cout << BLUE << "\nMood Insights:\n" << RESET;

        // Find the most common mood
        auto maxMood = std::max_element(moodCounts.begin(), moodCounts.end(),
            [](const auto& p1, const auto& p2) { return p1.second < p2.second; });

        std::cout << "Your most common mood: " << maxMood->first << "\n";

        // Calculate average happiness level
        int totalMoods = 0;
        int weightedHappiness = 0;
        std::map<std::string, int> moodWeights = {
            {"happy", 10}, {"sad", 2}, {"energetic", 8}, {"calm", 6},

            {"party", 9}, {"melancholy", 3}, {"motivational", 7},
            {"epic", 8}, {"relaxed", 7}, {"thoughtful", 6}
        };

        for (const auto& pair : moodCounts) {
            totalMoods += pair.second;
            weightedHappiness += pair.second * moodWeights[pair.first];
        }

        double averageHappiness = totalMoods > 0 ? static_cast<double>(weightedHappiness) / totalMoods : 5.0;
        std::cout << "Your average happiness level: " << std::fixed << std::setprecision(2) << averageHappiness << "/10\n";

        // Provide a mood-based recommendation
        std::cout << "\nBased on your mood history, we recommend:\n";
        if (averageHappiness < 5.0) {
            std::cout << "Consider listening to more uplifting and energetic music to boost your mood.\n";
        } else if (averageHappiness >= 5.0 && averageHappiness < 7.0) {
            std::cout << "Your mood seems balanced. Try exploring new genres to discover more music you might enjoy.\n";
        } else {
            std::cout << "You're in a great mood! Share your positive energy by creating and sharing playlists with friends.\n";
        }
    }

public:
    MoodPlaylistGenerator() : userHappinessLevel(5) {
        initializeSongDatabase();
        loadUserPreferences();
    }

    void run() {
        bool exitProgram = false;
        while (!exitProgram) {
            displayAsciiArt();
            displayHeader("AI Mood-Based Playlist Generator");

            std::cout << YELLOW << "1. Generate Playlist\n2. View Favorites\n3. Update Happiness Level\n"
                      << "4. View Most Played Songs\n5. View Mood Insights\n6. Exit\n" << RESET;
            int choice;
            std::cout << "Enter your choice: ";
            std::cin >> choice;

            switch (choice) {
                case 1: {
                    std::string mood = getUserMood();
                    simulateAIProcessing();
                    displayMoodAnalysis(mood);
                    std::vector<Song> playlist = generatePlaylist(mood, 5);
                    displayPlaylist(playlist);
                    provideMoodRecommendation(mood);

                    std::cout << YELLOW << "\nWould you like to add any songs to your favorites? (Enter song number, or 0 to skip): " << RESET;
                    int favoriteChoice;
                    std::cin >> favoriteChoice;
                    if (favoriteChoice > 0 && favoriteChoice <= static_cast<int>(playlist.size())) {
                        addToFavorites(playlist[favoriteChoice - 1], mood);
                    }
                    break;
                }
                case 2:
                    displayFavorites();
                    break;
                case 3:
                    std::cout << "Enter your current happiness level (1-10): ";
                    std::cin >> userHappinessLevel;
                    userHappinessLevel = std::max(1, std::min(10, userHappinessLevel));
                    break;
                case 4:
                    displayMostPlayedSongs();
                    break;
                case 5:
                    displayMoodInsights();
                    break;
                case 6:
                    exitProgram = true;
                    break;
                default:
                    std::cout << RED << "Invalid choice. Please try again.\n" << RESET;
            }

            if (!exitProgram) {
                std::cout << YELLOW << "\nPress Enter to continue..." << RESET;
                std::cin.ignore();
                std::cin.get();
            }
            clearScreen();
        }
        saveUserPreferences();
    }

    void clearScreen() {
        #ifdef _WIN32
        (void)std::system("cls");
        #else
        (void)std::system("clear");
        #endif
    }
};

int main() {
    MoodPlaylistGenerator generator;
    generator.run();
    return 0;
}