#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

inline std::vector<std::string> getBinFiles()
{
	std::vector<std::string> databases = std::vector<std::string>();
	std::string extension = ".bin";

	for (const auto& entry : fs::directory_iterator(fs::current_path())) {
		if (entry.is_regular_file() && entry.path().extension() == extension) {
			std::cout << entry.path().filename().string() << "\n";
			databases.push_back(entry.path().filename().string());
		}
	}
	return databases;

}

inline bool isAlphaString(const std::string& s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), [](unsigned char c) {
        return std::isalpha(c);
        });
}

inline std::string toUpperCopy(const std::string& s) {
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c) { return std::toupper(c); });
    return result;
}

inline std::string toLowerCopy(const std::string& s) {
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return result;
}

inline std::vector<std::string> split(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}