#ifndef LOGREADER_H
#define LOGREADER_H
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>


class LogReader {
public:
    explicit LogReader(std::string  fileName);
    void parseData();
    void graphOverTime(const std::string& category);
private:
    bool dataParsed;
    std::string fileName;
    std::vector<std::string> colNames;
    std::unordered_set<std::string> categories;
    std::unordered_set<std::string> numericalCategories;
    std::unordered_map<std::string, std::variant<std::vector<std::string>, std::vector<int>>> csvData;
    bool isCategory(const std::string& category) const;
    bool isIntegerCategory(const std::string& category) const;
    void convertStringsToInts(const std::string& category);
    void convertStringsToSeconds(const std::string& category);
};



#endif //LOGREADER_H
