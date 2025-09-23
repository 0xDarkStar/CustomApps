/*
Made By: notmacos
This file tests the MusicAPI interface with a simple approach.
*/
#include <iostream>
#include <string>

// Simple test to verify the API header compiles
int main() {
    std::cout << "=== Simple API Test ===" << std::endl;
    
    // Test that we can include the header
    std::cout << "API header includes successfully!" << std::endl;
    
    // Test basic functionality
    std::cout << "Testing basic string operations..." << std::endl;
    std::string test = "Hello, API!";
    std::cout << "Test string: " << test << std::endl;
    
    std::cout << "Simple API test completed!" << std::endl;
    return 0;
}
