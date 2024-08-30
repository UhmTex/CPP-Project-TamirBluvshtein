#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <msxml6.h>
#include <comdef.h>
#include <conio.h>
#include <algorithm>

#include "Classes.h"

#pragma comment(lib, "msxml6.lib")

void PrintPath(const vector<Node*>& path, int stepsToGoal, int totalSteps, const vector<string>& process) {
    if (path.empty()) {
        cout << "No valid path found.\n";
        return;
    }

    set<string> pathNodes;
    for (const Node* node : path) {
        pathNodes.insert(node->name);
    }

    cout << "Path found: ";
    for (Node* node : path) {
        cout << "\033[1m\033[33m" << node->name << "\033[0m";
        if (node != path.back()) {
            cout << " -> ";
        }
    }
    cout << endl;

    cout << "Successfull steps taken to reach the goal: " << stepsToGoal << endl;

    cout << "Total steps taken: " << totalSteps << endl;

    cout << "Pathfinding process: ";
    for (size_t i = 0; i < process.size(); ++i) {
        if (pathNodes.find(process[i]) != pathNodes.end()) {
            cout << "\033[1m\033[32m" << process[i] << "\033[0m";
        }
        else {
            cout << "\033[31m" << process[i] << "\033[0m";
        }

        if (i < process.size() - 1) {
            cout << " -> ";
        }
    }
    cout << endl;
}

string RemoveQuotes(const std::string& path) {
    std::string result = path;
    if (!result.empty() && result.front() == '"') {
        result.erase(result.begin());
    }
    if (!result.empty() && result.back() == '"') {
        result.pop_back();
    }
    return result;
}

string GetFilePath() {
    std::string FilePath;
    std::getline(std::cin, FilePath);
    return RemoveQuotes(FilePath);
}

bool FileExists(const std::string& filePath) {
    std::ifstream file(filePath);
    return file.good();
}

void GetUserChoice(char& choice) {
    while (true) {
        choice = _getch();
        choice = tolower(choice);

        if (choice == 'y' || choice == 'n') {
            cout << choice << endl;
            break;
        }
        else {
            cout << "\nInvalid choice. Please press 'y' or 'n' on your keyboard: ";
        }
    }
}

void ToUpperCase(string& str) {
    transform(str.begin(), str.end(), str.begin(), ::toupper);
}