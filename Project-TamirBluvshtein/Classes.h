#pragma once

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

#pragma comment(lib, "msxml6.lib")

using namespace std;

class Node {
public:
    string name;
    vector<Node*> neighbors;

    Node(string name) : name(name) {}

    void addNeighbor(Node* neighbor) {
        neighbors.push_back(neighbor);
    }
};

class Graph {
private:
    map<string, Node*> nodes;

    Graph() {}

public:
    static Graph& GetInstance() {
        static Graph instance;
        return instance;
    }

    void AddNode(string name) {
        if (nodes.find(name) == nodes.end()) {
            nodes[name] = new Node(name);
        }
    }

    void AddArc(string from, string to) {
        AddNode(from);
        AddNode(to);
        nodes[from]->addNeighbor(nodes[to]);
    }

    Node* GetNode(string name) {
        return nodes[name];
    }

    const map<string, Node*>& getNodes() const {
        return nodes;
    }

    void DrawGraph() const {
        cout << "Graph structure:" << endl;
        for (const auto& pair : nodes) {
            const Node* node = pair.second;
            cout << node->name << ": ";
            if (!node->neighbors.empty()) {
                for (size_t i = 0; i < node->neighbors.size(); ++i) {
                    cout << node->neighbors[i]->name;
                    if (i < node->neighbors.size() - 1) {
                        cout << ", ";
                    }
                }
            }
            cout << endl;
        }
    }

    ~Graph() {
        for (auto& pair : nodes) {
            delete pair.second;
        }
    }
};

class XMLGraphParser {
public:
    static bool ParseXML(const string& filename, Graph& graph) {

        // Used a little bit of online help for this part

        HRESULT hr = CoInitialize(NULL);
        if (FAILED(hr)) {
            cerr << "Failed to initialize COM library." << endl;
            return false;
        }

        IXMLDOMDocument* xmlDocument = nullptr;
        hr = CoCreateInstance(CLSID_DOMDocument60, NULL, CLSCTX_INPROC_SERVER, IID_IXMLDOMDocument, (void**)&xmlDocument);
        if (FAILED(hr)) {
            cerr << "Failed to create XML document instance." << endl;
            CoUninitialize();
            return false;
        }

        VARIANT_BOOL isSuccess;
        hr = xmlDocument->load(_variant_t(filename.c_str()), &isSuccess);
        if (FAILED(hr) || isSuccess != VARIANT_TRUE) {
            cerr << "Failed to load or parse XML file." << endl;
            xmlDocument->Release();
            CoUninitialize();
            return false;
        }

        IXMLDOMNodeList* nodeList = nullptr;
        xmlDocument->getElementsByTagName(_bstr_t("Node"), &nodeList);

        long length;
        nodeList->get_length(&length);

        for (long i = 0; i < length; i++) {
            IXMLDOMNode* node = nullptr;
            nodeList->get_item(i, &node);

            IXMLDOMNamedNodeMap* pAttributes = nullptr;
            node->get_attributes(&pAttributes);

            IXMLDOMNode* pNameAttr = nullptr;
            pAttributes->getNamedItem(_bstr_t("name"), &pNameAttr);

            BSTR nodeNameBSTR;
            pNameAttr->get_text(&nodeNameBSTR);
            string nodeName = (const char*)_bstr_t(nodeNameBSTR, false);

            graph.AddNode(nodeName);

            IXMLDOMNodeList* pArcList = nullptr;
            node->get_childNodes(&pArcList);

            long arcCount;
            pArcList->get_length(&arcCount);

            for (long j = 0; j < arcCount; j++) {
                IXMLDOMNode* pArcNode = nullptr;
                pArcList->get_item(j, &pArcNode);

                if (pArcNode) {
                    IXMLDOMNamedNodeMap* pArcAttributes = nullptr;
                    pArcNode->get_attributes(&pArcAttributes);

                    IXMLDOMNode* pTargetAttr = nullptr;
                    pArcAttributes->getNamedItem(_bstr_t("target"), &pTargetAttr);

                    BSTR targetNameBSTR;
                    pTargetAttr->get_text(&targetNameBSTR);
                    string targetName = (const char*)_bstr_t(targetNameBSTR, false);

                    graph.AddArc(nodeName, targetName);

                    pTargetAttr->Release();
                    pArcAttributes->Release();
                }
                pArcNode->Release();
            }

            pArcList->Release();
            pNameAttr->Release();
            pAttributes->Release();
            node->Release();
        }

        nodeList->Release();
        xmlDocument->Release();
        CoUninitialize();
        return true;
    }
};

class BFSPathfinder {
public:
    static vector<Node*> FindShortestPath(Node* start, Node* goal, int& stepsToGoal, int& totalSteps, vector<string>& process) {
        if (start == goal) return { start };

        map<Node*, Node*> cameFrom;
        map<Node*, int> distance;
        set<Node*> visited;

        queue<Node*> NodeQueue;
        NodeQueue.push(start);
        visited.insert(start);
        cameFrom[start] = nullptr;
        distance[start] = 0;

        totalSteps = 0;

        while (!NodeQueue.empty()) {
            Node* current = NodeQueue.front();
            NodeQueue.pop();
            process.push_back(current->name);

            totalSteps++;

            if (current == goal) {
                stepsToGoal = distance[current];
                break;
            }

            for (Node* neighbor : current->neighbors) {
                if (visited.find(neighbor) == visited.end()) {
                    visited.insert(neighbor);
                    NodeQueue.push(neighbor);
                    cameFrom[neighbor] = current;
                    distance[neighbor] = distance[current] + 1;
                }
            }
        }

        vector<Node*> path;
        Node* current = goal;

        if (cameFrom.find(current) == cameFrom.end()) {
            return path;
        }

        while (current != nullptr) {
            path.push_back(current);
            current = cameFrom[current];
        }

        reverse(path.begin(), path.end());
        return path;
    }
};
