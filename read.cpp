#include "billing.h"


int main(int argc, char **argv) {
	if (argc != 2) {
		std::cout << "Error in parameters" << std::endl;
		return -1;
	}
	int fd = open(argv[1], O_RDONLY);
	if (fd == -1) {
		std::cout << "Error,cant open file" << std::endl;
		return -1;
	}
	try {
		for (;;) {
			Billing billing;
			int re = billing.ReadBin(fd);
			if (re == 0)
				break;
			if (re == -1)
				throw std::invalid_argument("Error in read");
			billing.WriteTXT(stdout);
		}
	}
	catch(const std::exception &err) {
		std::cout << "Exception: " << err.what() << std::endl;
	}
	if (close(fd) == -1) {
		std::cout << "Error close file" << std::endl;
		return -1;
	}
	return 0;
}
