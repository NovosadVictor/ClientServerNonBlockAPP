#include "billing.h"
#include <ctime>


int main(int argc, char **argv) {
	if (argc != 3) {
		std::cout << "Error parameters" << std::endl;
		return -1;
	}
	int fd = open(argv[1], 577, 384);
	if (fd == -1) {
		std::cout << "Error, cant open file for writting" << std::endl;
		return -1;
	}
	srand(time(NULL));
	int quantity = atoi(argv[2]);
	try {
		Billing *checks = new Billing[quantity];
		for(int i = 0; i < quantity; ++i) {
			checks[i].SetRand();
			checks[i].WriteBin(fd);
		}
		if (close(fd) == -1) {
			std::cout << "Error in close" << std::endl;
			return -1;
		}
		delete[]checks;
	}
	catch (const std::exception& err) {
		std::cout << "Exception: " << err.what() << std::endl;
		return -1;
	}
	return 0;
}
