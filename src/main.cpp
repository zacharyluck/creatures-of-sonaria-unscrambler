#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <set>
#include <iterator>
#include <vector>

using namespace std;

class NameData {
public:
    string name;
    map<char, int> lettercounts;
};

vector<NameData> ReadNames(string path) {
    // Read file initially to get count for array
    fstream infile;
    // get current directory and work in it
    const string progname("COSHACK.exe");
    string newpath = path.substr(0, path.size() - progname.size());
    //cout << "Opening \"names.txt\" at: " << newpath << endl;

    infile.open(newpath + "names.txt", ios::in);
    if (!infile.is_open()) {
        perror("names.txt not found. ");
        exit(EXIT_FAILURE);
    }
    string line;
    vector<string> names;
    while (getline(infile, line)) {
        names.push_back(line);
        //cout << line << endl;
    }
    infile.close();
    //cout << "Names read." << endl;

    // Should have complete list of names, now
    // Convert it into an array of NameDatas
    vector<NameData> ret;
    for (int i=0; i<names.size(); i++) {
        //cout << "Processing Name " << i << ": " << names[i] << endl;
        // for each name in names
        // create letter array to count letters
        int l[28]{0};
        // iterate through the name, count all letters
        for (int j=0; j<names[i].length(); j++) {
            // check for spaces, count in index 26
            if (names[i][j] == ' ') {
                l[names[i][j] + 6] += 1;
            }
            // check for apostrophes, count in index 27
            else if (names[i][j] == '\'') {
                l[names[i][j] - 12] += 1;
            }
            // otherwise do the normal indexes
            else {
                l[names[i][j] - 97] += 1;
            }
            //cout << "Processing letter: " << names[i][j] << endl;
        }
        //cout << "Letters processed, assembling map for name." << endl;
        // should have letter counts, construct map for this name
        map <char, int> namemap;
        for (int j=0; j<sizeof(l); j++) {
            // create character from integer and input into map
            // when it gets to the last two indexes, have specific character inputs
            char c = j + 97;
            if (j == 26) {
                c = 20;
            }
            else if (j == 27) {
                c = 39;
            }
            namemap.insert({c, l[j]});
        }
        //cout << "Map created, adding to vector out." << endl;
        // should have a complete namemap, now construct the NameData
        NameData out;
        out.lettercounts = namemap;
        out.name = names[i];
        ret.push_back(out);
    }
    // should have a complete NameData array at this point
    //cout << "Names processed, leaving Readnames function..." << endl;
    return ret;
}

int main(int argc, char** argv) {
    //Set up lookup table to compare
    // check to make sure it's actually finding the files first
    //cout << "Starting ReadNames function..." << endl;
    vector<NameData> NameList = ReadNames(argv[0]);
    string scrambled = "";

    while (true) {
        cout << "Enter scrambled text: ";
        getline(cin, scrambled,'\n');

        if (scrambled == "&&") {
            exit(EXIT_SUCCESS);
        }

        // get letter info for scrambled text
        int l[28]{0};
        for (int i=0; i<scrambled.length(); i++) {
            // check for spaces, count in index 26
            if (scrambled[i] == ' ') {
                l[scrambled[i] + 6] += 1;
            }
            // check for apostrophes, count in index 27
            else if (scrambled[i] == '\'') {
                l[scrambled[i] - 12] += 1;
            }
            // otherwise do the normal indexes
            else {
                l[tolower(scrambled[i]) - 97] += 1;
            }
            //cout << "Processing letter: " << scrambled[i] << endl;
        }
        // should have letter counts now, now sort them by count
        // start by constructing a map for the sort function
        map<char, int> LetterCounts;
        // also save the size of array l
        int l_length = sizeof(l)/sizeof(l[0]);
        for (int i=0; i<l_length; i++) {
            char c = i + 97;
            if (i == 26) {
                c = 20;
            }
            else if (i == 27) {
                c = 39;
            }
            // don't need the info on letters that don't exist in the input
            if (l[i] != 0) {
                LetterCounts.insert({c, l[i]});
            }
        }
        // should have constructed map
        // go through name list one by one
        bool done = false;
        for (auto& i : NameList) {
            // go through each name one by one
            // break catch
            if (done) {
                break;
            }

            // step one: skip names of different lengths
            if (i.name.length() != scrambled.length()) {
                continue;
            }

            //cout << "checking name: " << i.name << endl;
            bool solved = true;
            for (auto& j : LetterCounts) {
                // at this point I know the following:
                // i should be a NameData
                // i.name is the name
                // i.lettercounts will return a map of all letter counts in the name
                // j should be a pair
                // j.first is a char
                // j.second is the number of characters in the inputted text

                //cout << j.first << ": " << i.lettercounts[j.first] << " vs " << j.second << endl;
                // step two, fail and break check
                if (i.lettercounts[j.first] != j.second) {
                    solved = false;
                    break;
                }
            }
            // if solved, set done to true
            if (solved) {
                done = true;
                cout << "The name is: " << i.name << endl << endl;
                break;
            }
        }
    }
    return 0;
}
