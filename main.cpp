#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

int main() {
	std::vector<std::string> forms;
	std::unordered_map<std::string, int> map;
	std::ifstream input;
	std::string uwi;
	std::string form;
	std::string tvd;
	std::string porosity;
	std::string comment;
	std::vector<std::string> lineData;
	bool formNoted = false;
	std::string line;
	int len;
	input.open("topsAccumapNPRACondensed.csv");
	//input.open("smalltest.csv");
	while (std::getline(input, line)) {
		
		//std::cout << line << std::endl;
		len = line.find(",");
		uwi = line.substr(0, len);
		if (uwi == "UWI" || uwi == "") continue;
		map[uwi]++;

		line = line.substr(len + 1, -1);
		len = line.find(",");
		form = line.substr(0, len);
		formNoted = false;

		for (std::string s : forms) {
			if (s == form) formNoted = true;
		}
		if (!formNoted) forms.push_back(form);
	}
	input.close();
	int max = 0;
	std::cout << "Done Loading File" << std::endl;
	for (const std::pair<const std::string, int>& n : map) {
		//std::cout << n.first << ", " << n.second << std::endl;
		if (n.second > max) max = n.second;
	}
	std::cout << "Max forms:" << max << std::endl;
	std::cout << "Forms: ";
	for (std::string f : forms) { std::cout << f; }
	std::cout << std::endl;

	
	std::unordered_map<std::string, std::vector<std::vector<std::string>>> geoscout;
	//input.open("smalltest.csv");
	input.open("topsAccumapNPRACondensed.csv");
	while (std::getline(input, line)) {
		lineData.clear();
		
		//std::cout << "original line " << line << std::endl;
		len = line.find(",");
		uwi = line.substr(0, len);
		if (uwi == "UWI" || uwi == "") continue;
		line = line.substr(len+1, -1);
		//std::cout << "new line " << line << std::endl;

		len = line.find(",");
		form = line.substr(0, len);
		line = line.substr(len+1, -1);
		//std::cout << "form " << form << std::endl;

		len = line.find(",");
		tvd = line.substr(0, len);
		line = line.substr(len+1, -1);

		len = line.find(",");
		porosity = line.substr(0, len);
		line = line.substr(len+1, -1);

		len = line.find(",");
		comment = line.substr(0, len);
		lineData.push_back(form);
		lineData.push_back(tvd);
		lineData.push_back(porosity);
		lineData.push_back(comment);
		geoscout[uwi].push_back(lineData);
		//std::cout << geoscout[uwi][0][0] << std::endl;
	}
	input.close();

	std::ofstream output("output.csv");
	output << "UWI,";
	for (int i = 0; i < forms.size(); i++) {
		output << forms[i]+" TVD," << forms[i] + " Porosity," << forms[i] + " Comment,";
	}
	output << "\n";

	bool formPosted = false;
	for (const std::pair<const std::string, std::vector<std::vector<std::string>>>& n : geoscout) {
		//std::cout << n.first << std::endl;
		//for (const std::vector<std::string> s : n.second) {
		//	std::cout << "uwi" << n.first << ", form: " << s[0] << ", tvd: " << s[1] << std::endl;
		//}
		output << n.first << ",";
		for (int i = 0; i < forms.size(); i++) {
			formPosted = false;
			for (const std::vector<std::string> s : n.second) {
				if (s[0] == forms[i]) {
					output << s[1]+"," << s[2] + "," << s[3] + ",";
					formPosted = true;
				}
			}
			if (!formPosted) output << ",,,";
		}
		output << "\n";
	}
}
