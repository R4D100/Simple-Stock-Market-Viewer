#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

const std::string apiKey = "YOUR_API_KEY";

struct StockData {
    std::string date;
    double open;
    double high;
    double low;
    double close;
    long volume;
};

size_t write_callback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string getStockData(const std::string& symbol, CURL* curl) {
    std::string response;
    std::ostringstream urlStream;
    urlStream << "https://www.alphavantage.co/query?function=TIME_SERIES_DAILY&symbol=" << symbol << "&apikey=" << apiKey;
    std::string url = urlStream.str();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
    }
    return response;
}

std::pair<double, std::string> findHighestPrice(const std::string& response) {
    double highestPrice = 0.0;
    std::string highestPriceDate;

    try {
        json parsedJson = json::parse(response);
        auto timeSeries = parsedJson["Time Series (Daily)"];

        for (const auto& item : timeSeries.items()) {
            double high = std::stod(item.value()["2. high"].get<std::string>());
            if (high > highestPrice) {
                highestPrice = high;
                highestPriceDate = item.key();
            }
        }
    }
    catch (const json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
        std::cerr << "Response: " << response << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return { highestPrice, highestPriceDate };
}

std::vector<StockData> parseJson(const std::string& response) {
    std::vector<StockData> stockDataList;

    try {
        json parsedJson = json::parse(response);
        auto timeSeries = parsedJson["Time Series (Daily)"];

        for (const auto& item : timeSeries.items()) {
            StockData data;
            data.date = item.key();
            data.open = std::stod(item.value()["1. open"].get<std::string>());
            data.high = std::stod(item.value()["2. high"].get<std::string>());
            data.low = std::stod(item.value()["3. low"].get<std::string>());
            data.close = std::stod(item.value()["4. close"].get<std::string>());
            data.volume = std::stol(item.value()["5. volume"].get<std::string>());

            stockDataList.push_back(data);
        }
    }
    catch (const json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
        std::cerr << "Response: " << response << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return stockDataList;
}

void printStockData(const std::vector<StockData>& stockDataList, double highestPrice, const std::string& highestPriceDate) {
    std::cout << "Daily Stock Data:" << std::endl;
    for (const auto& data : stockDataList) {
        std::cout << "Date: " << data.date << std::endl;
        std::cout << "Open: " << data.open << std::endl;
        std::cout << "High: " << data.high << std::endl;
        std::cout << "Low: " << data.low << std::endl;
        std::cout << "Close: " << data.close << std::endl;
        std::cout << "Volume: " << data.volume << std::endl;
        std::cout << "--------------------------" << std::endl;
    }

    // Print highest price information after the daily data
    std::cout << "The highest stock price ever: " << highestPrice << " on " << highestPriceDate << std::endl;
}

int main() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL* curl = curl_easy_init();

    if (!curl) {
        std::cerr << "Failed to initialize CURL" << std::endl;
        return 1;
    }

    std::string symbol;
    while (true) {
        std::cout << "Enter stock symbol (or 'quit' to exit): ";
        std::cin >> symbol;

        if (symbol == "quit") {
            break;
        }

        std::string response = getStockData(symbol, curl);
        if (response.empty()) {
            std::cerr << "No data received for symbol: " << symbol << std::endl;
            continue;
        }

        std::vector<StockData> stockDataList = parseJson(response);
        auto [highestPrice, highestPriceDate] = findHighestPrice(response);

        printStockData(stockDataList, highestPrice, highestPriceDate);
    }

    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return 0;
}
