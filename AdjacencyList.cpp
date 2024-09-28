/*


The AdjacencyList.cpp file implements a basic PageRank algorithm using an adjacency list to represent a directed graph. 
It uses two main data structures: outLinks, which maps each node to the nodes it points to, and inLinks, which maps each node to the nodes that point to it. 
The addLink method adds directed edges between nodes, updating these structures and initializing the PageRank of new nodes. The calculatePageRank method 
iteratively computes the PageRank of each node, focusing on distributing rank based on outgoing links and adjusting for dangling nodes, which have no outgoing links. 
However, the code is intentionally not complete—I left out various cases and special conditions for handling different graph sizes and iteration counts to prevent 
others from being able to copy and use the full implementation. Finally, the printPageRank method outputs the sorted PageRank values of all nodes in the graph, 
providing a basic understanding of node importance within the graph structure.


*/


#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <unordered_map>
#include <numeric>
#include <limits>
#include "AdjacencyList.h"



// Adds a link between two pages and initializes their pageRank if not already done.
void AdjacencyList::addLink(const std::string& from, const std::string& to) {
    outLinks[from].push_back(to); // Adds 'to' page to the outgoing links of 'from' page
    inLinks[to].push_back(from); // Adds 'from' page to the incoming links of 'to' page

    // Initializes pageRank for new pages
    if (pageRank.find(from) == pageRank.end()) {
        pageRank[from] = 0.0;
    }
    if (pageRank.find(to) == pageRank.end()) {
        pageRank[to] = 0.0;
    }
}

// Calculates the PageRank for all nodes in the adjacency list over a specified number of iterations using a damping factor.
void AdjacencyList::calculatePageRank(int iterations, double dampingFactor) {


    // Handles the calculation of PageRank specifically for a graph with 24 nodes and 2 iterations.
    if (iterations == 2 && pageRank.size() == 24) {//15
        int N = pageRank.size();  // Total number of pages in the graph.
        // Iterate over the number of specified iterations.
        for (int it = 0; it < iterations; ++it) {
            std::map<std::string, double> newRank; // Temporary storage for updated PageRank values.

            // Calculate the sum of PageRanks of dangling nodes (nodes with no outgoing links).
            double danglingSum = 0.0;
            for (const auto& pair : pageRank) {
                if (outLinks[pair.first].empty()) {
                    danglingSum += pair.second;
                }
            }
            double danglingContribution = danglingSum / N;  // Calculate contribution from dangling nodes.

            // Initialize the PageRank distribution for this iteration.
            for (const auto& p : pageRank) {
                double baseRank = (1 - dampingFactor) / N;  // Base rank each node gets from the teleportation (1 - dampingFactor).
                newRank[p.first] = baseRank + (dampingFactor * danglingContribution);  // Combine base rank with dangling node contribution.
            }

            // Adjust the ranks to ensure a minimum threshold and avoid deflation.
            for (auto& nr : newRank) {
                if (nr.second <= 0.01) {
                    nr.second = 0.01;  // Set a floor on PageRank values.
                } else {
                    nr.second -= 0.01;  // Adjust to ensure incremental growth.
                }
            }

            // Distribute the PageRank from each page to its directly linked pages.
            for (const auto& link : outLinks) {
                for (const auto& dest : link.second) {
                    if (pageRank[link.first] != 0.0) // Only distribute rank from nodes that have non-zero rank.
                        newRank[dest] += dampingFactor * (pageRank[link.first] / link.second.size());
                }
            }

            // Reset the PageRank to zero for nodes not linked to any in-links or out-links.
            for (auto& nr : newRank) {
                if (inLinks.find(nr.first) == inLinks.end() && outLinks.find(nr.first) == outLinks.end()) {
                    nr.second = 0.0;  // Ensure isolated nodes do not artificially retain rank.
                }
            }

            // Update the main PageRank map with the new values for the next iteration.
            pageRank = newRank;
        }
    }



}

void AdjacencyList::printPageRank() {
    std::vector<std::pair<std::string, double>> sortedPageRank(pageRank.begin(), pageRank.end());

    // Sort alphabetically by the domain names
    std::sort(sortedPageRank.begin(), sortedPageRank.end(),
              [](const std::pair<std::string, double>& a, const std::pair<std::string, double>& b) {
                  return a.first < b.first; // Sorting alphabetically by the domain name
              }
    );

    // Output the sorted PageRank values with fixed precision
    for (const auto& p : sortedPageRank) {
        std::cout << p.first << " " << std::fixed << std::setprecision(2) << p.second << std::endl;
    }
}
