#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <curl/curl.h>

bool stop = false;

size_t getInfoFunction(void* pointer, size_t size, size_t n, std::string* data) {
    data->append((char*)pointer, size * n);
    return size * n;
}

//gets location data from ipinfo.io/.
std::string* getCurlData(std::string& input){
    CURL *curl = curl_easy_init();
    CURLcode res;
    std::string* output = new std::string;
    std::string url = "ipinfo.io/" + input + "?token=6a75cd72870a5a"; //replace with your token if needed
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getInfoFunction);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &output);
    res = curl_easy_perform(curl); 
    curl_easy_cleanup(curl);
    curl = NULL;
    return output;
}

//helper for the parse function; locBuffer is roughly the length of toFind.
std::string* findInString(std::string* output, std::string toFind, int locBuffer){
    int loc = output->find(toFind) + locBuffer;
    int newline = output->find("\n", loc);
    std::string* value = new std::string(output->substr(loc, newline - loc));
    *value = value->substr(3, value->length() - 5);
    return value;
}

//puts the curl data in an actually useful format.
std::string* parseCurlData(std::string* output){
    std::string* city = findInString(output, "\"city\": ", 6);
    std::string* region = findInString(output, "\"region\": ", 8);
    std::string* country = findInString(output, "\"country\": ", 9);
    std::string* location = findInString(output, "\"loc\": ", 5);
    std::string* returnString = new std::string(*city + "; " + *region + ", " + *country + ". " + *location);
    delete[](city);
    delete[](region);
    delete[](country);
    delete[](location);
    delete[](output);
    return returnString;
}

//does exactly what the function name implies; helper function for getLocation. input is an IP address.
std::string* getLocationData(std::string& input){
    if (input == "stop"){
        stop = !stop;
        return &input;
    }
    else{
        std::string* curlData = getCurlData(input);
        std::string* returnString = parseCurlData(curlData);
        delete[](curlData);
        return returnString;
    }
}

//checks if input is actually an IP address
bool validateInput(std::string& input){
    if (input == "stop"){
        return true;
    }
    else if (input.length() > 3 && input.length() < 40){
        if (input[0] > 47 && input[0] < 58){
            return true;
        }
        else return false;
    }
    else return false;
}

//also does exactly what the function name implies.
std::string* getLocation(){
    std::fstream file;
    std::string input = "";
    file.open("pipe.txt");
    if (!file.is_open()){std::cout << "error opening file" << std::endl;}
    file >> input;
    if (validateInput(input)){
        std::string* returnString = getLocationData(input);
        file.close();
        return returnString;
    }
    file.close();
    return new std::string("not IP");
}

int main(){
    while(!stop){
        std::string* output = getLocation();
        if(!stop){
            std::ofstream outFile;
            outFile.open("pipe.txt");
            outFile << *output << std::flush;
            outFile.close();
        }
        delete[](output);
    }
    return 0;
};