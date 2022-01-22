// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h" // The file containing the list of words. 
// #include "Math/UnrealMathUtility.h"

void UBullCowCartridge::BeginPlay() // When the game starts.
{
    Super::BeginPlay(); // Please enter your code underneath this otherwise you will get weird errors.

    PrintLine(TEXT("JO MAMA Game is Gae!")); // When you press the play button this will be printed in terminal.

    Isograms = GetValidWords(Words); // We call the GetValidWords() here once only and use this array. So that everytime when we want a random word, we don't need to call GetValidWords() again and again.

    SetupGame();  
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter, code here will execute. (Referencing the user's input,
{ 
    // When game is over.
    if (bGameOver)
    {
        ClearScreen(); // Entering sth to the terminal will clear the screen. 
        SetupGame(); 
    }

    // Game is ongoing.
    else  
    {
        // Print the first 5 words of the array Words. 
        /*
        for (int i = 0; i < 5; i++) {
            PrintLine(TEXT("%s"), *Words[i]); // Remember to dereference the the array Words. [Yes need to dereference twice because this is a 2D array of characters.]
        }
        */

        // Print from the array all the words that are length of 5. 
        /*
        for (int i = 0; i < Words.Num(); i++) {
            if (Words[i].Len() == 5) {
                PrintLine(TEXT("%s"), *Words[i]);
            }
        }
        */

        ProcessGuess(PlayerInput); // Checking PlayerGuess.
    }
}

void UBullCowCartridge::SetupGame()
{
    // Welcoming The Player
    PrintLine(TEXT("Welcome to Bull Cows!"));
    
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)]; // Note that GetValidWords() returns an array of valid words. Which you can dereference it like array. And must minus 1 because index starts from zero...
    Lives = HiddenWord.Len() * 2; // Sets up the life. 
    bGameOver = false; // Game is ongoing... 
    
    PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len()); // .len() prints out the length of the string. 
    PrintLine(TEXT("You have %i lives."), Lives);
    PrintLine(TEXT("Type in your guess and \npress enter to continue...")); // Prompt Player For Guess.
    PrintLine(TEXT("The HiddenWord is: %s."), *HiddenWord); // Debug Line to display the hidden word. 
    PrintLine(TEXT("Yah I given you the answer are you not happy?"));
    PrintLine(TEXT("Nice Cow Sex btw..."));
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true; // Member function can use the attributes.
    PrintLine(TEXT("\nPress enter to play again."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if (Guess == HiddenWord)
    {
        PrintLine(TEXT("You have Won!")); 
        EndGame();
        return;
    }

    if (Guess.Len() != HiddenWord.Len()) // When the Guess length is not correct. 
    {
        PrintLine(TEXT("The hidden word is %i letters long."), HiddenWord.Len());
        PrintLine(TEXT("Sorry, try guessing again! \nYou have %i lives remaining."), Lives);
        return;
    }

    // Check If Isogram
    if (!IsIsogram(Guess))
    {
        /* code */
        PrintLine(TEXT("No repeating letters, guess again!"));
        return;
    }

    // Remove Life
    PrintLine(TEXT("Lost a life!"));
    --Lives;

    if (Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You have no lives left!"));
        PrintLine(TEXT("The hidden word was: %s"), *HiddenWord);
        EndGame();
        return;
    }

    // Show the player Bulls and Cows
    FBullCowCount Score = GetBullCows(Guess); // This is a struct, see this at BullCowCartridge.h

    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);

    PrintLine(TEXT("Guess again, you have %i lives left"), Lives);
}

// Checks for repeated words. 
bool UBullCowCartridge::IsIsogram(const FString& Word) const // This function is not changing anything in the members. So put const, but must also put const for the func declaration of the header file. 
{

    for (int32 Index = 0; Index < Word.Len(); Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index] == Word[Comparison])
            {
                return false;
            }
        }   
    }

    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;
    
    // Range based for loop.   
    for (FString Word : WordList) // Iterating variable is Word.
    {
        if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word)) // This checks if length is between 4 and 8 and is isogram!
        {
            ValidWords.Emplace(Word); // CTRL + Click on Emplace to see what it does.     
        }
    }
    return ValidWords;
}

// Bull is for number of correct letters at the correct position.
// Cow is number of correct letters at wrong position. 
FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++; // Count is a struct and changing it within this const method is fine since it is not a member object of the class of this method.
            continue; // Skips this current outer for loop index and go to the next outer for loop index. 
        }
        
        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                Count.Cows++;
                break; // Straight away break out of this inner for loop. 
            }
        }
    }
    return Count;
}