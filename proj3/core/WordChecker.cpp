// WordChecker.cpp
//
// ICS 46 Winter 2019
// Project #3: Set the Controls for the Heart of the Sun
//
// Replace and/or augment the implementations below as needed to meet
// the requirements.

#include "WordChecker.hpp"



WordChecker::WordChecker(const Set<std::string>& words)
    : words{words}
{
}


bool WordChecker::wordExists(const std::string& word) const
{
	if(words.contains(word))
    	return true;
    else
    	return false;
}

void WordChecker::swapAdjacent(const std::string& word, std::vector<std::string>& suggestions) const
{
	for (int i = 0; i < word.length() - 1; ++i)
	{
		std::string temp = word;
		std::swap(temp[i], temp[i+1]);
		if(wordExists(temp) && std::find(suggestions.begin(), suggestions.end(), temp) == std::end(suggestions))
			suggestions.push_back(temp);
	}
}

void WordChecker::insertChar(const std::string& word, std::vector<std::string>& suggestions) const
{
	for (int i = 0; i < word.length() + 1; ++i)
	{
		
		char c = 'A';
		while(c <= 'Z')
		{
			std::string temp = word;
			temp = temp.insert(i, 1, c);
			if(wordExists(temp) && std::find(suggestions.begin(), suggestions.end(), temp) == std::end(suggestions))
				suggestions.push_back(temp);
			c++;
		}
		
	}
	
}

void WordChecker::deleteChar(const std::string& word, std::vector<std::string>& suggestions) const
{
	for (int i = 0; i < word.length(); ++i)
	{
		std::string temp = word;
		temp.erase(i, 1);
		if(wordExists(temp) && std::find(suggestions.begin(), suggestions.end(), temp) == std::end(suggestions))
			suggestions.push_back(temp);
	}
}

void WordChecker::replaceChar(const std::string& word, std::vector<std::string>& suggestions) const
{
	for (int i = 0; i < word.length(); ++i)
	{
		char c = 'A';
		while(c <= 'Z')
		{
			std::string temp = word;
			temp.replace(i, 1, 1, c);
			if(wordExists(temp) && std::find(suggestions.begin(), suggestions.end(), temp) == std::end(suggestions))
				suggestions.push_back(temp);
			c++;
		}
		
	}
}

void WordChecker::spaceBetween(const std::string& word, std::vector<std::string>& suggestions) const
{
	for (int i = 0; i < word.length(); ++i)
	{
		std::string s1 = word.substr(0, i+1);
		std::string s2 = word.substr(i+1, word.length()-i-1);
		std::string combine = s1 + " " + s2;
		if(wordExists(s1) && wordExists(s2))
		{
			suggestions.push_back(combine);
		}
	}
}

std::vector<std::string> WordChecker::findSuggestions(const std::string& word) const
{
	//std::string copy = word;
	//std::transform(copy.begin(), copy.end(), copy.begin(), ::tolower);
	std::vector<std::string> suggestions;
	swapAdjacent(word, suggestions);
    insertChar(word, suggestions);
    deleteChar(word, suggestions);
    replaceChar(word, suggestions);
    spaceBetween(word, suggestions);
    return suggestions;
}

