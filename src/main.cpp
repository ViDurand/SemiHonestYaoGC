
#include "../include/YaoParties.hpp"

int main(int argc, char* argv[]) {

    CmdParser parser;
    auto parameters = parser.parseArguments("",argc, argv);
    int partyNum = stoi(parser.getValueByKey(parameters, "partyID"));


	if (partyNum == 0) {
		// create Party one with the previous created objects.
		PartyOne p1(argc, argv);
        // run Party one
        p1.run();
	}
	else if (partyNum == 1) {
		PartyTwo p2(argc, argv);
        // run party two of Yao protocol.
        p2.run();
		auto output = p2.getOutput();

	} else {
		std::cerr << "Usage: ./SemiHonestYaoGC <partyID(0|1)> <config/file/path>" << std::endl;
		return 1;
	}

	return 0;
}

