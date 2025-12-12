#include "pch.h"
#include "ThreadManager.h"

#include <iostream>

int main()
{
	std::cout << "Server is running..." << std::endl;
	// Server logic goes here
	int8 t = 0;

	std::cout << CPP_VERSION << " " << __cplusplus << std::endl;

	GThreadManager->Launch([]()
		{
			for (int i = 0; i < 1'000'000; ++i) {
				std::cout << "i: " << i << std::endl;
			}
		});
}
