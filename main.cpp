#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <map>
#include <unordered_set>
#include <vector>


// ��������� �������� ����� � ���������� ����� �� ������ ��������� regex
// ����:
// - inputFileName: ��� ����� ��� ���������
// - allowedDomains: ����� ����������� �������� �������
// - outputFiles: �����, ���������� �������� ����� ��� ������ �������
// - otherFile: ����� �������� ���� ��� ����� � ��������, �� ��������� � allowedDomains
void processFile(const std::string &inputFileName, const std::unordered_set<std::string> &allowedDomains,
                 std::map<std::string, std::ofstream> &outputFiles, std::ofstream &otherFile) {
    std::ifstream inputFile(inputFileName);
    if (!inputFile.is_open()) {
        std::cerr << "�� ������� ������� ����: " << inputFileName << std::endl;
        return;
    }

    std::string line;
    std::regex emailRegex(R"((\w+)(\.\w+)*@(\w+\.)+\w+)");
    std::smatch match;
    while (std::getline(inputFile, line)) {
        if (std::regex_search(line, match, emailRegex)) {
            //�� @
            std::string email = match.str();
            size_t pos = email.find('@');
            std::string domain = email.substr(pos + 1);
            //�� .
            pos = domain.find('.');
            domain = domain.substr(0, pos);
            std::transform(domain.begin(), domain.end(), domain.begin(), tolower);

            std::ofstream *outputStream = nullptr;
            if (allowedDomains.find(domain) != allowedDomains.end()) {
                if (!outputFiles.contains(domain)) {
                    outputFiles[domain] = std::ofstream(domain + ".txt", std::ios::app);
                    if (!outputFiles[domain].is_open()) {
                        std::cerr << "�� ������� ������� ���� ��� ������: " << domain + ".txt" << std::endl;
                        continue;
                    }
                }
                outputStream = &outputFiles[domain];
            } else {
                outputStream = &otherFile;
            }

            pos = line.find(':');
            if (pos != std::string::npos) {
                std::string emailPart = line.substr(0, pos);
                std::string rest = line.substr(pos + 1);
                *outputStream << emailPart << "\t" << rest << std::endl;
            } else {
                *outputStream << line << std::endl;
            }
        }
    }
}


int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "Russian");
    if (argc < 2) {
        std::cerr << "�������������: " << argv[0] << " <������ ������>" << std::endl;
        return 1;
    }

    std::vector<std::string> allowedDomain =
    {
        "aliceadsl",
        "bellsouth",
        "bigpond",
        "bluewin",
        "blueyonder",
        "centurytel",
        "charter",
        "chello",
        "club-internet",
        "comcast",
        "earthlink",
        "facebook",
        "freenet",
        "frontiernet",
        "gmail",
        "googlemail",
        "hetnet",
        "hotmail",
        "laposte",
        "libero",
        "mail",
        "ntlworld",
        "optonline",
        "optusnet",
        "orange",
        "outlook",
        "planet",
        "rambler",
        "rediffmail",
        "rocketmail",
        "sbcglobal",
        "skynet",
        "sympatico",
        "t-online",
        "telenet",
        "tiscali",
        "verizon",
        "virgilio",
        "voila",
        "wanadoo",
        "windstream",
        "yahoo",
        "yandex",
        "zonnet",
        "zoho",
    };
    // ����������� ������ ����������� ������� � unordered_set ��� ������������ ������
    std::unordered_set allowedDomains_set(allowedDomain.begin(), allowedDomain.end());

    // ����� ��� �������� �������� ������ ��� ������� ������
    std::map<std::string, std::ofstream> outputFiles;
    std::ofstream otherFile("other.txt", std::ios::app);

    if (!otherFile.is_open()) {
        std::cerr << "�� ������� ������� ���� ��� ������: Other.txt" << std::endl;
        return 1;
    }

    // ��������� ���� ������� ������
    int totalFiles = argc - 1;
    for (int i = 1; i < argc; ++i) {
        std::cout << "��������� ����� " << argv[i] << " (" << i << " �� " << totalFiles << ")..." << std::endl;
        processFile(argv[i], allowedDomains_set, outputFiles, otherFile);
        float progress = static_cast<float>(i) / static_cast<float>(totalFiles) * 100;
        std::cout << "��������: " << progress << "%" << std::endl;
    }

    for (auto &pair: outputFiles) {
        pair.second.close();
    }
    otherFile.close();

    std::cout << "��������� ���������." << std::endl;
    return 0;
}
