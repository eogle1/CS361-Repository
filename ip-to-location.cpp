#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <curl/curl.h>

size_t getInfoFunction(void* pointer, size_t size, size_t n, std::string* data) {
    data->append((char*)pointer, size * n);
    return size * n;
}

int main(){
    bool stop = false;
    std::fstream file;
    std::string input = "";
    while(!stop){
        file.open("pipe.txt");
        if (!file.is_open()){
            std::cout << "error opening file" << std::endl;
        }
        file >> input;
        if (((input.length() > 3 && input.length() < 40) && (input[0] > 47 && input[0] < 58)) || input == "stop"){
            if (input == "stop"){
                stop = !stop;
            }
            else{
                CURL *curl = curl_easy_init();
                CURLcode res;
                std::string output;
                std::string url = "ipinfo.io/" + input + "?token=6a75cd72870a5a"; //replace with your token
                curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getInfoFunction);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &output);
                res = curl_easy_perform(curl); 
                curl_easy_cleanup(curl);
                curl = NULL;
                file.close();
                std::ofstream outFile;
                outFile.open("pipe.txt");

                //parse response
                int loc = output.find("\"city\": ") + 6;
                int newline = output.find("\n", loc);
                std::string city = output.substr(loc, newline - loc);
                city = city.substr(3, city.length() - 5);

                loc = output.find("\"region\": ") + 8;
                newline = output.find("\n", loc);
                std::string region = output.substr(loc, newline - loc);
                region = region.substr(3, region.length() - 5);

                loc = output.find("\"country\": ") + 9;
                newline = output.find("\n", loc);
                std::string country = output.substr(loc, newline - loc);
                country = country.substr(3, country.length() - 5);

                loc = output.find("\"loc\": ") + 5;
                newline = output.find("\n", loc);
                std::string location = output.substr(loc, newline - loc);
                location = location.substr(3, location.length() - 5);

                output = city + "; " + region + ", " + country + ". " + location;
                outFile << output << std::flush;
                outFile.close();
                file.open("pipe.txt");
            }
        }
        file.close();
    }
    return 0;
};