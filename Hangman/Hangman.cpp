// Hangman.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
#include <cctype>
using namespace std;

int main()
{
    // inizalize variables
    int guessIncorrect = 0;
    string randomWord;
    vector<string> words{"SWORD", "ARMOR", "SHEILD"}; // string
    srand((time(0)));
    int randomNum = rand() % 3;
    randomWord = words[randomNum];
    int guessRemaining = randomWord.size();
    string guessingWord;
    cout << "Hangman" << "\n" << "guess the word" << "\n";
        int loop = 1;
        for (int i = 0; i < loop; i++) {
            char letter;
            cout << "guesses made: " << loop - 1 << "\n";
            cout << "guesses incorrect: " << guessIncorrect << "\n";
            cout << "guesses remaining: " << guessRemaining << "\n";
            cout << "guess a letter: ";
            cin >> letter;
            letter = toupper(letter);
            if (randomWord.find(letter) != string::npos && guessingWord.find(letter) == string::npos) {
                if (guessRemaining != 1) {
                    guessingWord = guessingWord + letter + "-";
                }
                else {
                    guessingWord = guessingWord + letter;
                }
                cout << guessingWord << "\n";
                guessRemaining--;
            }
            else if (guessingWord.find(letter) != string::npos) {
                cout << "Already Guessed" << "\n";
            }
            else {
                guessIncorrect++;
                cout << "Not in the word" << "\n";
            }
            if (!(guessIncorrect >= randomWord.size()) && !(guessRemaining <= 0)) {
                loop++;
            }
            else if(guessIncorrect >= randomWord.size()){
                cout << "YOU LOST" << "\n" << "guessed to many letters wrong" << "\n";
            }
            else {
                cout << "YOU WON word is: " << randomWord;
            }

        }

}