#include <fstream>
#include <iostream>
#include <set>
#include <string>

int main(int argc, char** argv) {
	if (argc != 2) {
		throw std::runtime_error{"no make compilation log specified"};
	}

	std::ifstream input{std::string{argv[1]}};
	std::set<std::string> compile_commands;
	std::string word;
	auto isystem = false;
	while (input >> word) {
		if (word.size() <= 2) {
			continue;
		}

		std::string prefix = word.substr(0, 2);
		if (prefix == "-I" || prefix == "-W" || prefix == "-f" || prefix == "-s") {
			compile_commands.emplace(word);
		} else if (prefix == "-i") {
			isystem = true;
		} else if (isystem) {
			isystem = false;
			compile_commands.emplace("-I" + word);
		}
	}

	std::ofstream output{".clang_complete", std::ofstream::trunc};
	for (auto& compile_command : compile_commands) {
		output << compile_command << std::endl;
	}

	return 0;
}
