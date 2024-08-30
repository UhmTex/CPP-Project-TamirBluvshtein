#include <iostream>
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
#include "Methods.h"

#pragma comment(lib, "msxml6.lib")

using namespace std;

int main() {
    string xmlFilePath;
    Graph& graph = Graph::GetInstance();

    bool validFile = false;

    while (!validFile) {
        cout << "Enter the path to the XML file: ";
        xmlFilePath = GetFilePath();

        if (!FileExists(xmlFilePath)) {
            std::cerr << "Invalid file path! Please try again." << std::endl;;
        }
        else {

            validFile = XMLGraphParser::ParseXML(xmlFilePath, graph);
            if (!validFile) {
                std::cerr << "Failed to parse XML file. Please check the file and try again";
            }
            else {
                graph.DrawGraph();
            }
        }
    }

    char continuePathfinding = 'y';
    while (continuePathfinding == 'y') {
        cout << "-------------------------------------------------\n";
        string startNode, endNode;
        cout << "Enter the start node: ";
        cin >> startNode;
        cout << "Enter the end node: ";
        cin >> endNode;

        ToUpperCase(startNode);
        ToUpperCase(endNode);

        Node* start = graph.GetNode(startNode);
        Node* goal = graph.GetNode(endNode);

        if (!start || !goal) {
            cerr << "Invalid start or end node. Please try again.\n";
            continue;
        }

        int stepsToGoal = 0;
        int totalSteps = 0;
        vector<string> process;
        vector<Node*> path = BFSPathfinder::FindShortestPath(start, goal, stepsToGoal, totalSteps, process);

        if (path.empty()) {
            cout << "No valid path found. Please try different start and end nodes.\n";
        }
        else {
            PrintPath(path, stepsToGoal, totalSteps, process);
        }

        cout << "\nRestart? (y/n): ";
        GetUserChoice(continuePathfinding);
    }

    return 0;
}
