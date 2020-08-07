/*hash.cpp*/

//
//<<Akshaj Uppala>>
// Prof. Joe Hummel
// U. of Illinois, Chicago
// CS 251: Spring 2020
// 
// Project 6 part 2
// 

#include <iostream>
#include <string>
#include <cctype>  /*isdigit*/
#include <regex>   /*regular expressions*/

#include "hash.h"

using namespace std;

//
// isNumeric
//
// Returns true if the given string is numeric (all digits), false
// if not.  If the string is empty, false is returned since there 
// are no digits present.
//
bool isNumeric(string s)
{
	//
	// A string is numeric if it contains 1 or more digits, so let's
	// use a regular expression to check that.
	//
	// we are using POSIX basic notation for regular expressions, see
	//   https://en.cppreference.com/w/cpp/regex/ecmascript
	// where [:d:] means digit, [[:d:]] means match a single digit.
	// The + means match the preceding sub-expression 1 or more times.
	//
	regex pattern("[[:d:]]+", regex::ECMAScript);

	smatch matchResults;  // provides more details on the match

	if (regex_match(s, matchResults, pattern))
		return true;
	else
		return false;
}

//Four different hash functions for the four different hashmaps
//
//
int HashByStationID(int id, int N){
	
	return id % N;
}

//Dan J. Bernstein has written this algorithm
//This is a Multiplicative string hash function
int HashByStationAbbrev(string abbrev, int N){
	
	unsigned long long stringHash = 5381;
	for(size_t i = 0; i < abbrev.length(); ++i){
		stringHash = (stringHash*33)+abbrev[i];
	}
	
	return stringHash % N;
}

int HashByTripID(string tripId, int N){
	tripId.erase(0,2);
	int index = stoi(tripId);
	
	return index%N;
}

int HashByBikeID(string bikeId, int N){
	bikeId.erase(0,1);
	int index = stoi(bikeId);
	
	return index%N;
}