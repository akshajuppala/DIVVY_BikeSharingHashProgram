/*hash.h*/

//<<Akshaj Uppala>>
// Prof. Joe Hummel
// U. of Illinois, Chicago
// CS 251: Spring 2020
// 
// Project 6 part 2
// 

#include <string>

using namespace std;

bool isNumeric(string s);
int HashByStationID(int id, int N);
int HashByStationAbbrev(string abbrev, int N);
int HashByTripID(string tripId, int N);
int HashByBikeID(string bikeId, int N);