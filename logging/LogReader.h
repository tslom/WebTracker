#ifndef LOGREADER_H
#define LOGREADER_H
#include <string>
#include <vector>
#include <unordered_map>


class LogReader {
public:
    explicit LogReader(const std::string& fileName);
    bool parseData();
private:
    std::string fileName;
    std::vector<std::string> colNames;
    std::unordered_map<std::string, std::vector<std::string>> csvData;

};



#endif //LOGREADER_H
