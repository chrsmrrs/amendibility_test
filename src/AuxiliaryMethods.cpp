/**********************************************************************
 * Copyright (C) 2017 Christopher Morris <christopher.morris@udo.edu>
 *
 * This file is part of Amenable. An implementation of the algorithm described in:
 *
 * @InProceedings{Arvind+2015,
 *   author = "Arvind, V. and K{\"o}bler, Johannes and Rattan, Gaurav and Verbitsky, Oleg",
 *   title = "On the Power of Color Refinement",
 *   booktitle = "20th International Symposium on Fundamentals of Computation Theory",
 *   year = "2015",
 *   pages = "339--350"
 * }
 *
 * Amenable can not be copied and/or distributed without the express
 * permission of Christopher Morris.
 *********************************************************************/

#include "AuxiliaryMethods.h"

using namespace std;

namespace AuxiliaryMethods {
    vector<int> split_string(string s) {
        vector<int> result;
        stringstream ss(s);

        while (ss.good()) {
            string substr;
            getline(ss, substr, ',');
            result.push_back(stoi(substr));
        }

        return result;
    }

    GraphDatabase read_graph_txt_file(string data_set_name) {
        string line;
        vector<uint> graph_indicator;
        ifstream myfile(
                "/home/morris/amenability_test/data_sets/" + data_set_name + "/" + data_set_name +
                "_graph_indicator.txt");
        if (myfile.is_open()) {
            while (getline(myfile, line)) {
                graph_indicator.push_back(stoi(line) - 1);
            }
            myfile.close();
        } else {
            printf("%s", "!!! Unable to open file !!!\n");
            exit(EXIT_FAILURE);
        }

        uint num_graphs = graph_indicator.back() + 1;

        // Get labels from for each node.
        bool label_data = true;
        string label;
        Labels node_labels;
        ifstream labels(
                "/home/morris/amenability_test/data_sets/" + data_set_name + "/" + data_set_name + "_node_labels.txt");
        if (labels.is_open()) {
            while (getline(labels, label)) {
                node_labels.push_back(stoul(label));
            }
            myfile.close();
        } else {
            label_data = false;
        }

        GraphDatabase graph_database;
        unordered_map<int, int> offset;
        int num_nodes = 0;

        // Add vertices to each graph in graph database and assign labels.
        for (uint i = 0; i < num_graphs; ++i) {
            pair<int, int> p(i, num_nodes);
            offset.insert(p);
            unsigned long s = count(graph_indicator.begin(), graph_indicator.end(), i);

            Labels l;
            if (label_data) {
                for (unsigned long j = num_nodes; j < s + num_nodes; ++j) {
                    l.push_back(node_labels[j]);
                }
            }

            num_nodes += s;
            EdgeList edge_list;

            Graph new_graph(s, edge_list, l);
            graph_database.push_back(new_graph);
        }

        // Insert edges for each graph.
        vector<int> edges;
        ifstream edge_file(
                "/home/morris/amenability_test/data_sets/" + data_set_name + "/" + data_set_name + "_A.txt");
        if (edge_file.is_open()) {
            while (getline(edge_file, line)) {
                vector<int> r = split_string(line);

                uint graph_num = graph_indicator[r[0] - 1];
                uint off = offset[graph_num];
                Node v = r[0] - 1 - off;
                Node w = r[1] - 1 - off;

                if (!graph_database[graph_num].has_edge(v, w)) {
                    graph_database[graph_num].add_edge(v, w);
                }
                edges.push_back(stoi(line));
            }
            edge_file.close();
        } else {
            printf("%s", "!!! Unable to open file !!!\n");
            exit(EXIT_FAILURE);
        }

        return graph_database;
    }
}
