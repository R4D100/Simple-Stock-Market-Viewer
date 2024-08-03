# Stock Market Tracker

This project is a stock market tracker that fetches historical stock data using the Alpha Vantage API and analyzes it to find the highest stock price and its corresponding date.

## Features

- Fetch historical daily stock data for a given symbol.
- Display detailed stock information, including the highest price ever recorded.
- Handle multiple stock symbols and display results consecutively.

## Prerequisites

Before you start, ensure you have the following installed:

- **C++ Compiler**: Ensure you have a C++ compiler like `g++` or `clang++`.
- **CURL Library**: Used for making HTTP requests.
- **JSON for Modern C++**: A header-only JSON library for parsing and serializing JSON.

## Installation

1. **Install CURL Library**

   - **Ubuntu/Debian**: 
     ```bash
     sudo apt-get install libcurl4-openssl-dev
     ```
   - **macOS**: 
     ```bash
     brew install curl
     ```
   - **Windows**: Download and install the CURL binaries from the [official CURL website](https://curl.se/windows/).

2. **Install JSON for Modern C++**

   Download the `json.hpp` file from the [nlohmann/json GitHub repository](https://github.com/nlohmann/json) and place it in your project directory.

## API Key

To use the Alpha Vantage API, you'll need an API key. Replace `YOUR_API_KEY` in the code with your actual API key. You can obtain an API key by signing up for a free account at [Alpha Vantage](https://www.alphavantage.co/support/#api-key).
