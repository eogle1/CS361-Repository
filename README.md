# CS361-Repository
Welcome to my repository for CS361. I am currently hosting my microservice here, and will place my main program in a separate repository.\
This microservice takes an IP address and returns a location.
## How to request data
This microservice continually checks a text file called 'pipe.txt' (without the quotes). In order to call it, you write an IP address to the text file with this name. An example of this in c++ is:\
oFile << "1.40.154.210"; \
with oFile being an ofstream object that has opened pipe.txt.\
The call itself is the '1.40.154.210' section (without the quotes, once again). I consider it up to you to get this into the text file where the microservice can read it.
## How to receive data
The microservice will write the result in the same text file, overwriting the IP address that was previously there. It is on a single line, and formatted as:
### City; Region, CountryAbbreviation. LatitudeValue,LongitudeValue
One example of how to receive this response via c++ is:\
getline(iFile, str);\
with iFile being an ifstream object that has opened pipe.txt, and str being an empty string.
## UML Diagram
Currently in progress.

