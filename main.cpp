#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

int main() {
	std::vector<std::string> forms;
	std::vector<std::string> uwis;
	std::ifstream input;

	std::string uwi;
	std::string form;
	std::string tvd;
	std::string porosity;
	std::string comment;
	std::string line;
	int len;
	std::vector<std::string> lineData;

	bool formNoted = false;
	bool uwiNoted = false;
	
	std::cout << "Loading file input.csv..." << "\n";

	//load the accumap csv into a map with uwi, [[form1,tvd,porosity,comment],[form2,tvd,porosity,comment],[...]...]
	std::unordered_map<std::string, std::vector<std::vector<std::string>>> geoscout;
	input.open("input.csv");
	while (std::getline(input, line)) {
		lineData.clear();

		len = line.find(",");
		uwi = line.substr(0, len);
		if (uwi == "UWI" || uwi == "") continue; //dont read the first line
		line = line.substr(len + 2, -1);

		len = line.find(",");
		form = line.substr(0, len);
		line = line.substr(len + 1, -1);

		len = line.find(",");
		tvd = line.substr(0, len);
		line = line.substr(len + 1, -1);

		len = line.find(",");
		porosity = line.substr(0, len);
		line = line.substr(len + 1, -1);

		len = line.find(",");
		comment = line.substr(0, len);

		lineData.push_back(form);
		lineData.push_back(tvd);
		lineData.push_back(porosity);
		lineData.push_back(comment);
		geoscout[uwi].push_back(lineData);

		//add form to list of forms if not on list
		formNoted = false;
		for (int i = 0; i < forms.size(); i++) {
			if (forms[i] == form) {
				formNoted = true;
				break;
			}
		}
		if (!formNoted) {
			forms.push_back(form);
			std::cout << "Found form: " + form << "\n";
		}

	}
	input.close();

	std::cout << "Done Loading File" << "\n";
	std::cout << "Num of forms: " << forms.size() << "\n";
	std::cout << "Forms: ";
	for (std::string f : forms) std::cout << f;
	std::cout << "\n";
	std::cout << "Num of uwis: " << geoscout.size() << "\n";

	//print list of forms to a file named formsList.csv
	std::ofstream formsList("formsList.csv");
	for (int i = 0; i < forms.size(); i++) {
		formsList << forms[i] + ", ";
		if (i % 10 == 0) formsList << "\n";
	}
	formsList.close();

	//change the order of the forms in the csv, first will be shown first, etc...
	std::vector<std::string> formsOrdered = {
		"BLSK","BLSK G/WC",
		"GTNG","GTNG CHNL","GTNG LWR","GTNG BSL","GTNG MID","GTNG COAL","GTNG G/WC","GTNG B","GTNG SHAL","GTNG C","GTNG A","GTNG MRKA","GTNG MRKB",
		"FRNI MRKA",
		"NRDG","NRDG E","NRDG D",
		"CLLK D","CLLK","CLLK C","CLLK B","CLLK MRKA","CLLK MRKB","CLLK LWR","CLLK E",
		"NCYS",
		"CECS",
		"COPU","COPU BASE","COPU LWR",
		"BDLK","BDLK BASE","BDLK ZMRK",
		"HLFY","HLFY A","HLFY G/WC","HLFY MRKA","HLFY PORS","HLFY CHNL",
		"DOIG","DOIG BASE","DOIG LWR","DOIG ZMRK","DOIG TPOR","DOIG A","DOIG CHNL","DOIG BPOR","DOIG G/WC","DOIG SHAL","DOIG MRKA",
		"MNTN","MNTN SHAL","MNTN BPOR","MNTN A","MNTN O/WC","MNTN B","MNTN G/OC","MNTN MID","MNTN MRKA","MNTN PORS","MNTN G/WC","MNTN MRKB",
		"BLLY","BLLY G/WC","BLLY MRKA"};

	for (int i = 0; i < forms.size(); i++) {
		formNoted = false;
		for (int j = 0; j < formsOrdered.size(); j++) {
			if (formsOrdered[j] == forms[i]) {
				formNoted = true;
				break;
			}
			
		}
		if (!formNoted) formsOrdered.push_back(forms[i]);
	}
	//if some of the forms in my "priority order" are not listed in your csv, this number will be larger than "num of forms"
	std::cout << "Num of forms after priority order: " << formsOrdered.size() << "\n";


	std::cout << "Outputting output.csv... \n";
	std::ofstream output("output.csv");
	//create the header with the column names
	output << "UWI,";
	for (int i = 0; i < formsOrdered.size(); i++) {
		output << formsOrdered[i]+" TVD," << formsOrdered[i] + " Porosity," << formsOrdered[i] + " Comment,";
	}
	output << "\n";

	bool formPosted = false;
	int outputIterator = 0;
	int uwiPerPercent = geoscout.size() / 100;
	for (const std::pair<const std::string, std::vector<std::vector<std::string>>>& n : geoscout) {
		output << n.first << ",";
		for (int i = 0; i < formsOrdered.size(); i++) {
			formPosted = false;
			for (const std::vector<std::string> s : n.second) {
				if (s[0] == formsOrdered[i]) {
					output << s[1]+"," << s[2] + "," << s[3] + ",";
					formPosted = true;
				}
			}
			if (!formPosted) output << ",,,";
		}
		output << "\n";
		outputIterator++;
		if (outputIterator % uwiPerPercent == 0) {
			std::cout << "Outputting " << (outputIterator * 100) / geoscout.size() << "%\n";
		}
	}
	output.close();
	std::cout << "Done outputting!" << "\n";
}
