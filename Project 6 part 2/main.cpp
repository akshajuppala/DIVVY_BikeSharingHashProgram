/*main.cpp*/

//<<Akshaj Uppala>>
// Prof. Joe Hummel
// U. of Illinois, Chicago
// CS 251: Spring 2020\
// 
// Project 6 part 2
// 

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <queue>
#include <utility>
#include <set>

#include "hash.h"
#include "hashmap.h"
#include "util.h"

using namespace std;

//Struct for storing the each station's data individually
struct StationData
{
	int ID;
	string Abbrev;
	string Name;
	double Latitude;
	double Longitude;
	int Capacity;
	string OnlineDate;
	
	StationData(){
		ID = -1;
		Abbrev = "";
		Name = "";
		Latitude = -999.0;
		Longitude = -999.0;
		Capacity = -1;
		OnlineDate = "";
	}
};

//Struct for storing every trip's data
struct TripData
{
	string TripID;
	string StartTime;
	string StopTime;
	string BikeID;
	int Duration;
	int FromStationID;
	int ToStationID;
	string Gender;
	int BirthYear;
	
	TripData(){
		TripID = "";
		StartTime = "";
		StopTime = "";
		BikeID = "";
		Duration = -1;
		FromStationID = -1;
	    ToStationID = -1;
		Gender = "";
		BirthYear = -1;
	}
};

//
// string2int
// 
// Converts a string to an integer, unless string is empty, in
// which case 0 is returned.
// 
int string2int(string s)
{
	if (s == "")
		return 0;
	else
		return stoi(s);
}

//
// inputData
// 
// Given the filenames denoting stations and trips,
// inputs that data into the given hash tables.
// 
int inputData(string stationsFile, string tripsFile, hashmap<int, StationData>& stationsById, hashmap<string, int>& stationsByAbbrev, hashmap<string,TripData>& tripsById, hashmap<string,int>& bikesById)
{
	// --- OPENING THE STATIONS FILE --- //
	// 
	// 
	ifstream  infile(stationsFile);
	
	cout << "Reading " << stationsFile << endl;
	
	if (!infile.good())
	{
		cout << "**Error: unable to open '" << stationsFile << "'..." << endl;
		return 1;
	}
	
	// file is open, start processing:
	string line;
	
	getline(infile, line);  // input and discard first row --- header row
	
	int numStations = 0;
	
	while (getline(infile, line))
	{
		stringstream s(line);
		
		string id, abbrev, name, latitude, longitude, capacity, online_date;
		
		getline(s, id, ',');  
		getline(s, abbrev, ',');   
		getline(s, name, ',');
		getline(s, latitude, ',');
		getline(s, longitude, ',');
		getline(s, capacity, ',');
		getline(s, online_date);
		
		//
		// store into hash table using a StationData object
		// 
		StationData SD;
		
		SD.ID = stoi(id);
		SD.Abbrev = abbrev;
		SD.Name = name;
		SD.Latitude = stod(latitude);
		SD.Longitude = stod(longitude);
		SD.Capacity = stoi(capacity);
		SD.OnlineDate = online_date;

		//inserting into stationsById and stationsByAbbrev
		stationsById.insert(stoi(id), SD, HashByStationID);
		stationsByAbbrev.insert(abbrev, stoi(id), HashByStationAbbrev);
		
		numStations++;
	}
	
	// --- OPENING THE TRIPS FILE --- //
	// 
	// 
	
	ifstream  inFile(tripsFile);
	
	cout << "Reading " << tripsFile << endl;
	
	if (!inFile.good())
	{
		cout << "**Error: unable to open '" << tripsFile << "'..." << endl;
		return -1;
	}
	
	// file is open, start processing:
	line = "";
	
	getline(inFile, line);  // input and discard first row --- header row
	
	int numTrips = 0;
	int numBikes = 0;
	
	while (getline(inFile, line))
	{
		stringstream s(line);
		
		string tripId, startTime, stopTime, bikeId, duration, fromStationId, toStationId, gender, birthYear;
		
		getline(s, tripId, ',');  
		getline(s, startTime, ',');   
		getline(s, stopTime, ',');
		getline(s, bikeId, ',');
		getline(s, duration, ',');
		getline(s, fromStationId, ',');
		getline(s, toStationId, ',');
		getline(s, gender, ',');
		getline(s, birthYear, ',');
		
		//
		// store into hash table using a TripData object
		// 
		TripData TD;

		TD.TripID = tripId;
		TD.StartTime = startTime;
		TD.StopTime = stopTime;
		TD.BikeID = bikeId;
		TD.Duration = stoi(duration);
		TD.FromStationID = stoi(fromStationId);
		TD.ToStationID = stoi(toStationId);	
		TD.Gender = gender;
		if(birthYear == "")
			TD.BirthYear = -1;
		else
			TD.BirthYear = stoi(birthYear);

		//inserting all the trips in tripsById
		tripsById.insert(tripId, TD, HashByTripID);
		int numUsages = -1;
		bool bikeFound = bikesById.search(bikeId, numUsages, HashByBikeID);
		//A bike is inserted in the bikesById only when a new bike is found
		if(bikeFound == false){
			numUsages = 1;
			bikesById.insert(bikeId, numUsages, HashByBikeID);
			numBikes++; //increasing the number of bikes every time a new id is encountered
		}
		else{
			numUsages++;
			//bikeId is repetitevly inserted into the hashmap with the increased numUsage every time an already used bike is encountered
			bikesById.insert(bikeId, numUsages, HashByBikeID);
		}
		
		numTrips++;
	}
	
	cout << endl;
	cout << "# of stations: " << numStations << endl;
	cout << "# of trips: " << numTrips << endl;
	cout << "# of bikes: " << numBikes << endl;
	
	return 0;  // we have data to be processed:
}

//Command 1: For displaying the info about each station using stationsById
void DisplayStationInfo(int id, hashmap<int, StationData> stationsById){
	StationData temp;
	bool success = stationsById.search(id, temp, HashByStationID);
	
	if(success == false){
		cout << "Station not found" << endl;
		return;
	}
	
	cout << "Station:" << endl;
	cout << " ID: " << temp.ID << endl;
	cout << " Abbrev: " << temp.Abbrev << endl;
	cout << " Fullname: " << temp.Name << endl;
	cout << " Location: (" << temp.Latitude << ", " << temp.Longitude << ")" << endl;
	cout << " Capacity: " << temp.Capacity << endl;
	cout << " Online date: " << temp.OnlineDate << endl;
	
}

//command 2: For displaying the info about each station using the stationsByAbbrev
void DisplayStationInfoThroughAbbrev(string abbrev, hashmap<int, StationData> stationsById, hashmap<string, int> stationsByAbbrev){
	int id = -1;
	bool success = stationsByAbbrev.search(abbrev, id, HashByStationAbbrev);
	
	if(success)
		DisplayStationInfo(id, stationsById); //we use the previous function as soon as we get the id from the stationsByAbbrev
	else
		cout << "Station not found" << endl;
}

//command 3: For displaying the info about each station using the tripsById
void DisplayTripInfo(string tripId, hashmap<string,TripData> tripsById, hashmap<int, StationData> stationsById){
	TripData temp;
	bool success = tripsById.search(tripId, temp,HashByTripID);
	
	if(success){
		cout << "Trip:" << endl;
		cout << " ID: " << temp.TripID << endl;
		cout << " Starttime: " << temp.StartTime << endl;
		cout << " Bikeid: " << temp.BikeID << endl;
		cout << " Duration: ";
		if(temp.Duration > 3600)
			cout << temp.Duration/3600 << " hours, " << (temp.Duration%3600)/60 << " minutes, " << (temp.Duration%3600)%60 << " seconds" << endl;
		else if(temp.Duration > 60)
			cout << temp.Duration/60 << " minutes, " << temp.Duration%60 << " seconds" << endl;
		else
			cout << temp.Duration << " seconds" << endl;
		
		StationData SD;
		stationsById.search(temp.FromStationID, SD, HashByStationID);
		cout << " From Station: " << SD.Abbrev << " (" << temp.FromStationID << ")" << endl;
		stationsById.search(temp.ToStationID, SD, HashByStationID);
		cout << " To Station: " << SD.Abbrev << " (" << temp.ToStationID << ")" << endl;
		
		cout << " Rider identifies as: " << temp.Gender << endl;
		if(temp.BirthYear == -1)
			cout << " Birthyear: " << endl;
		else
			cout << " Birthyear: " << temp.BirthYear << endl;
	}
	else
		cout << "Trip not found" << endl;
	
}

//command 4: Displaying the bike info using the bikesById 
void DisplayBikeInfo(string bikeId, hashmap<string,int> bikesById){
	
	int numUsages = -1;
	bool success = bikesById.search(bikeId, numUsages, HashByBikeID);
	if(success){
		cout << "Bike: " << endl;
		cout << " ID: " << bikeId << endl;
		cout << " Usage: " << numUsages << endl;
	}
	else{
		cout << "Bike not found" << endl;
	}
}

//function to implement the priority_queue
class prioritize
{
public:
	bool operator()(const pair<int,double>& p1, const pair<int, double>& p2) const
	{
		//Distance is allotted as the values and the order is based on the values
		if(p1.second>p2.second){
			return true;
		}
		//if two distances are same, the keys are allotted in ascending order
		else if(p1.second == p2.second && p1.first>p2.first){
			return true;
		}
		else 
			return false;
	}
	
};

//Command 5: Displaying Nearby stations
void DisplayNearbyStations(string command, hashmap<int, StationData> stationsById){
	
	stringstream s(command);
	string commandName, latitude, longitude, numMiles;
	getline(s, commandName, ' ');
	getline(s, latitude, ' ');
	getline(s, longitude, ' ');
	getline(s, numMiles);
	double latitudeVal = stod(latitude);
	double longitudeVal = stod(longitude);
	double numMilesVal = stod(numMiles);
	vector<int> keysStations = stationsById.GetKeys();
	StationData SD;
	//using the priority queue to display nearest stations in the order
	priority_queue<pair<int, double>, vector<pair<int, double>>, prioritize> stations;
	
	for(size_t i = 0; i < keysStations.size(); ++i){
		stationsById.search(keysStations.at(i), SD, HashByStationID);
		double distance = distBetween2Points(latitudeVal, longitudeVal, SD.Latitude, SD.Longitude);
		
		if(distance < numMilesVal){
			stations.push(make_pair(SD.ID,distance)); //add in the queue only if the distance requirement is satisfied
		}
	}
	
	
	cout << "Stations within " + numMiles + " miles of (" + latitude + ", " + longitude + ")" << endl;
	
	if(stations.empty()){
		cout << " none found" << endl;
		return;
	}
	//sort(stations.begin(), stations.begin()+stations.size());
	
	
	while(!stations.empty()){
		pair<int, double> stationNearby = stations.top();
		stations.pop();
		cout << " station " << stationNearby.first << ": " << stationNearby.second << " miles" << endl;
	}
}

//Command 6 : Displaying trips similar to the trip given by the user
void DisplaySimilarTrips(string command, hashmap<int, StationData> stationsById, hashmap<string,TripData>& tripsById){
	stringstream s(command);
	string commandName, tripId, numMiles;
	getline(s, commandName, ' ');
	getline(s, tripId, ' ');
	getline(s, numMiles);
	int numTrips = 0;
	double numMilesVal = stod(numMiles);
	
	cout << "Trips that follow a similar path (+/-" << numMiles << " miles) as " << tripId << endl;
	
	TripData originalTrip;
	bool success = tripsById.search(tripId, originalTrip, HashByTripID);
	if(!success){
		cout << " no such trip" << endl;
		return;
	}
	
	int FromStationId = originalTrip.FromStationID;
	int ToStationId = originalTrip.ToStationID;
	StationData FromStation, ToStation;
	stationsById.search(FromStationId, FromStation, HashByStationID);
	stationsById.search(ToStationId, ToStation, HashByStationID);
	
	//Locations of the from and to stations of the trips
	double FromStationLatitude = FromStation.Latitude;
	double FromStationLongitude = FromStation.Longitude;
	double ToStationLatitude = ToStation.Latitude;
	double ToStationLongitude = ToStation.Longitude;
	
	set<int> FromStations;
	set<int> ToStations;
	
	vector<int> stationKeys = stationsById.GetKeys();
	cout << " nearby starting points:";
	
	//Loop for forming the sets of the S' and D' as explained in the pdf
	for(size_t i = 0; i < stationKeys.size(); ++i){
		StationData SD;
		
		stationsById.search(stationKeys.at(i), SD, HashByStationID);
		double distance = distBetween2Points(FromStationLatitude, FromStationLongitude, SD.Latitude, SD.Longitude);
		if(distance < numMilesVal){
			FromStations.insert(stationKeys.at(i));
			cout << " " << stationKeys.at(i);
		}
		
		distance = distBetween2Points(ToStationLatitude, ToStationLongitude, SD.Latitude, SD.Longitude);
		if(distance < numMilesVal){
			ToStations.insert(stationKeys.at(i));
		}
	}
	cout << endl;
	cout << " nearby ending points:";
	for(int e: ToStations){
		cout << " " << e;
	}
	
	vector<string> tripKeys = tripsById.GetKeys();
	for(size_t i = 0; i < tripKeys.size(); ++i){
		TripData TD;
		
		tripsById.search(tripKeys.at(i), TD, HashByTripID);
		if(FromStations.count(TD.FromStationID)) //If the given trip's from station is present in the S' set(Here FromStations set)
			if(ToStations.count(TD.ToStationID)) //If the given trip's to station is present in the D' set (Here ToStations set)
				numTrips++;
		
	}
	cout << endl;
	cout << " trip count: " << numTrips << endl;
}

//Function to display help
void DisplayHelp(){
	cout << "Available commands:" << endl;
	cout << " Enter a station id (e.g. 341)" << endl;
	cout << " Enter a station abbreviation (e.g. Adler)" << endl;
	cout << " Enter a trip id (e.g. Tr10426561)" << endl;
	cout << " Enter a bike id (e.g. B5218)" << endl;
	cout << " Nearby stations (e.g. nearby 41.86 -87.62 0.5)" << endl;
	cout << " Similar trips (e.g. similar Tr10424639 0.3)" << endl;
}
int main()
{
	cout << "** DIVVY analysis program **" << endl;
	cout << endl;

	//A load factor of 5 has been used
	hashmap<int, StationData> stationsById(10000);     //because 2000 stations are assumed
	hashmap<string, int> stationsByAbbrev(10000);      //because 2000 stations are assumed
	hashmap<string,TripData> tripsById(2500000);       //because 500000 trips are assumed
	hashmap<string,int> bikesById(50000);              //because 10000 bikes are assumed
	
	//
	// Input file names:
	// 
	string stationsFile;
	string tripsFile;
	cout << "Enter stations file> ";
	getline(cin, stationsFile);
// 	if(stationsFile == "")
// 		stationsFile = "stations.csv";
	cout << "Enter trips file> ";
	getline(cin, tripsFile);
// 	if(tripsFile == "")
// 		tripsFile = "trips.csv";
	cout << endl;
	
	int success = inputData(stationsFile, tripsFile, stationsById, stationsByAbbrev, tripsById, bikesById);
	
	//
	// did we input anything?
	// 
	if (success != 0)
	{
		if(success == 1)
			cout << "No data, Stations file not found?" << endl;
		if(success == -1)
			cout << "No data, Trips file not found?" << endl;
		return 0;
	}
	
	//
	// allow the user to enter command:
	// 
	string command;
	
	cout << endl;
	cout << "Please enter a command, help, or #> ";
	getline(cin, command);

	while (command != "#")
	{
		//
		// we have a command, let's look in hash table and
		// see if we have any data:
		// 
		if(isNumeric(command)){
			DisplayStationInfo(stoi(command), stationsById);
		}
		else if(command.substr(0,2) == "Tr" && isNumeric(command.substr(2,command.length()-2))){
			DisplayTripInfo(command, tripsById, stationsById);
		}
		else if(command[0] == 'B' && isNumeric(command.substr(1, command.length()-1))){
			DisplayBikeInfo(command, bikesById);
		}
		else if(command.substr(0,7) == "nearby "){
			DisplayNearbyStations(command, stationsById);
		}
		else if(command.substr(0,8) == "similar "){
			DisplaySimilarTrips(command, stationsById, tripsById);
		}
		else if(command == "help"){
			DisplayHelp();
		}
		else{
			DisplayStationInfoThroughAbbrev(command, stationsById, stationsByAbbrev);
		}
		
		cout << endl;
		cout << "Please enter a command, help, or #> ";
		getline(cin, command);
	}
	
	
	return 0;
}
