#ifndef LOGREADER_H
#define LOGREADER_H
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <unordered_set>

/**
 * @class LogReader
 * @brief Reads CSV logs and performs data analysis and visualization.
 */
class LogReader {
public:
    /**
     * @brief Constructs a LogReader for the specified log file.
     * @param fileName Path to the log file.
     */
    explicit LogReader(std::string  fileName);
    /**
     * @brief Parses the CSV data into internal data structures.
     */
    void parseData();
    /**
     * @brief Displays a time-series graph of the selected category.
     * @param category Field to graph (e.g., "PacketSize", "Time").
     */
    void graphOverTime(const std::string& category);
    /**
     * @brief Graphs a mapped integer category over time (e.g., protocols).
     * @param category Field to graph.
     */
    void graphOverTimeWithMap(const std::string& category);
    /**
     * @brief Prints a list of integer-mappable categories.
     */
    void printIntCategories();
private:
    bool dataParsed;
    std::string fileName;
    std::vector<std::string> colNames;

    std::unordered_set<std::string> categories;
    std::unordered_set<std::string> intCategories;
    std::unordered_map<std::string, std::variant<std::vector<std::string>, std::vector<int>>> csvData;

    std::map<std::string, std::variant<std::vector<std::string>, std::vector<int>>> csvDataWithMap;

    /**
     * @brief Checks if the category exists in the parsed CSV.
     * @param category Column name.
     * @return True if valid.
     */
    bool isCategory(const std::string& category) const;

    /**
     * @brief Checks if the category is suitable for integer mapping.
     * @param category Column name.
     * @return True if valid.
     */
    [[nodiscard]] bool isIntCategory(const std::string& category) const;

    /**
     * @brief Maps strings in the given category to integers.
     * @param category Column name.
     * @param map Whether to print the mapping.
     */
    void convertStringsToInts(const std::string& category, bool map);

    /**
     * @brief Converts time strings to seconds from the initial time.
     * @param category Time column name.
     * @param map Whether to print the mapping.
     */
    void convertStringsToSeconds(const std::string& category, bool map);
};



#endif //LOGREADER_H
