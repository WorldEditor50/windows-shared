#ifndef CSV_HPP
#define CSV_HPP
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

class CSV
{
private:
    std::string fileName;
    std::string delimeter;
public:
    explicit CSV(const std::string &fileName_,
                 const std::string &delimeter_=",")
        :fileName(fileName_), delimeter(delimeter_){}

    ~CSV(){}
    void parseLine(const std::string &strLine, std::vector<std::string> &vecCells)
    {
        std::string str = strLine;
        while (str.find(delimeter.c_str()) != std::string::npos) {
            unsigned int nPos = str.find(delimeter.c_str());
            std::string strCell = str.substr(0, nPos);
            str = str.substr(nPos + 1, str.size());
            vecCells.push_back(strCell);
        }
        vecCells.push_back(str);
        return;
    }

    void makeLine(const std::vector<std::string> &vecCells, std::string &strLine)
    {
        for (std::vector<std::string>::const_iterator iter = vecCells.begin(); iter != vecCells.end(); ++iter) {
            std::string strCell = *iter;
            strLine += strCell;
            if ((iter + 1) != vecCells.end()) {
                strLine += delimeter;
            }
        }
        return;
    }

    void appendLine(const std::string &strLine)
    {
        std::ofstream ofs;
        std::locale loc = std::locale::global(std::locale(""));
        ofs.open(fileName.c_str(), std::ios::out | std::ios::app);
        std::locale::global(loc);
        ofs << strLine << std::endl;
        ofs.close();
        return;
    }

    void appendLines(const std::vector<std::string> &vecLines)
    {
        std::ofstream ofs;
        std::locale loc = std::locale::global(std::locale(""));
        ofs.open(fileName.c_str(), std::ios::out | std::ios::app);
        std::locale::global(loc);
        for (std::vector<std::string>::const_iterator iter = vecLines.begin(); iter != vecLines.end(); ++iter) {
            ofs << *iter << std::endl;
        }
        ofs.close();
        return;
    }

    void writeLines(const std::vector<std::string> &vecLines)
    {
        std::ofstream ofs;
        std::locale loc = std::locale::global(std::locale(""));
        ofs.open(fileName.c_str(), std::ios::out | std::ios::trunc);
        std::locale::global(loc);
        for (std::vector<std::string>::const_iterator iter = vecLines.begin(); iter != vecLines.end(); ++iter) {
            ofs << *iter << std::endl;
        }
        ofs.close();
        return;
    }

    std::string readFirstLine()
    {
        std::ifstream ifs;
        std::locale loc = std::locale::global(std::locale(""));
        ifs.open(fileName.c_str(), std::ios::in);
        std::locale::global(loc);
        std::string strLine;
        std::getline(ifs, strLine);
        return strLine;
    }

    void readLines(std::vector<std::string> &vecLines)
    {
        std::ifstream ifs;
        std::locale loc = std::locale::global(std::locale(""));
        ifs.open(fileName.c_str(), std::ios::in);
        std::locale::global(loc);

        std::string strLine;
        while (std::getline(ifs, strLine)) {
            vecLines.push_back(strLine);
        }
        return;
    }

};

#endif // CSV_HPP
