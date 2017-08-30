#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "simpio.h"
#include "console.h"
#include "filelib.h"
#include "strlib.h"
#include "stack.h"
#include "queue.h"
#include "set.h"

using namespace std;

void printInstructions();

void loadDictionary(Set<string> &dictionary);

void buildLadder(Set<string> &dictionary);

void promptUserForWords(string &w1, string &w2, Set<string> &dictionary);

int main() {
    Set<string> dictionary;
    printInstructions();
    loadDictionary(dictionary);
    buildLadder(dictionary);
    cout << "Have a nice day." << endl;
    return 0;
}

void printInstructions() {
    cout << "Welcome to CS 106B Word Ladder." << endl;
    cout << "Please give me two English words, and I will change the" << endl;
    cout << "first into the second by changing one letter at a time." << endl;
    cout << endl;
}

void loadDictionary(Set<string> &dictionary) {
    string input;
    while (true) {
        input = getLine("Dictionary file name? ");
        if (fileExists(input)) break;
        cout << "Unable to open that file. Try again." << endl;
    }
    cout << endl;
    ifstream infile;
    infile.open(input.c_str());
    while (!infile.fail()) {
        string word;
        getline(infile, word);
        dictionary.add(word);
    }
}

void buildLadder(Set<string> &dictionary) {
    while (true) {
        string alpha = "abcdefghijklmnopqrstuvwxyz";
        Set<string> usedWords;
        Queue<Stack<string>> ladders;
        Stack<string> solutionLadder;
        string w1;
        string w2;
        promptUserForWords(w1, w2, dictionary);
        if (w1 == "" || w2 == "") break;
        Stack<string> initialStack;
        initialStack.add(w1);
        ladders.enqueue(initialStack);
        while (!ladders.isEmpty()) {
            Stack<string> currentStack = ladders.dequeue();
            string currentWord = currentStack.peek();
            for (int i = 0; i < currentWord.length(); i++) {
                for (int j = 0; j < 26; j++) {
                    string neighborWord;
                    if (i == 0) {
                        neighborWord = alpha[j] + currentWord.substr(i + 1);
                    }
                    if (i > 0 && i < currentWord.length() - 1) {
                        neighborWord = currentWord.substr(0, i) + alpha[j] + currentWord.substr(i + 1);
                    }
                    if (i == currentWord.length() - 1) {
                        neighborWord = currentWord.substr(0, i) + alpha[j];
                    }
                    if (dictionary.contains(neighborWord) && neighborWord != w1) {
                        if (!usedWords.contains(neighborWord)) {
                            if (neighborWord == w2) {
                                currentStack.push(neighborWord);
                                solutionLadder = currentStack;
                                ladders.clear();
                                break;
                            } else {
                                Stack<string> archiveStack = currentStack;
                                archiveStack.push(neighborWord);
                                ladders.enqueue(archiveStack);
                                usedWords.add(neighborWord);
                            }
                        }
                    }
                }
            }
        }
        string solutionLadderString = "";
        while (!solutionLadder.isEmpty()) {
            solutionLadderString += solutionLadder.pop() + " ";
        }
        cout << "A ladder from " << w2 << " back to " << w1 << ":" << endl;
        cout << solutionLadderString << endl;
    }
}

void promptUserForWords(string &w1, string &w2, Set<string> &dictionary) {
    while (true) {
        w1 = getLine("Word #1 (or Enter to quit): ");
        if (w1 == "") break;
        w2 = getLine("Word #2 (or Enter to quit): ");
        if (w2 == "") break;
        w1 = toLowerCase(w1);
        w2 = toLowerCase(w2);
        if (w1 != w2 && w1.length() == w2.length() && dictionary.contains(w1) && dictionary.contains(w2)) break;
        if (w1 == w2) {
            cout << "The two words must be different." << endl;
        } else {
            if (w1.length() != w2.length()) {
                cout << "The two words must be the same length." << endl;
            } else {
                if (!(dictionary.contains(w1) && dictionary.contains(w2))) {
                    cout << "The two words must be found in the dictionary." << endl;
                }
            }
        }
        cout << endl;
    }
}
