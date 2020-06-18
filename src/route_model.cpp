#include "route_model.h"
#include <iostream>

RouteModel::RouteModel(const std::vector<std::byte> &xml) : Model(xml) {
    // Create RouteModel nodes.
    int counter = 0;
    for (Model::Node node : this->Nodes()) {
        m_Nodes.emplace_back(Node(counter, this, node));
        counter++;
    }
    //call function in constructor
    CreateNodeToRoadHashmap();
}


void RouteModel::CreateNodeToRoadHashmap() {
    //loop iterates through the vector given by calling Road()
    for (const Model::Road &road : Roads()) {

	//for each ref in vector road check that type is not footway 
        if (road.type != Model::Road::Type::Footway) {

	    //loop over each node_idx in the way that road belong to Ways 
            for (int node_idx : Ways()[road.way].nodes) {

		//if node index is not node_to _road hashmap yet 
                if (node_to_road.find(node_idx) == node_to_road.end()) {

		    //set value for key_idx to be empty vector of object
                    node_to_road[node_idx] = std::vector<const Model::Road *> ();
                }
		//push pointer to current road in the back of vector given by node_idx key in node
                node_to_road[node_idx].push_back(&road);
            }
        }
    }
}


RouteModel::Node *RouteModel::Node::FindNeighbor(std::vector<int> node_indices) {
	//start with pointer Node 
    Node *closest_node = nullptr;
	//update 
    Node node;
	//lop through the node indices vector to find the closed unvisited node
	//for each indix in loop  
    for (int node_index : node_indices) {

	    //you can relative the node object with Snode
        node = parent_model->SNodes()[node_index];

		//for each relative Node in loop check that the node ha been not visitedand distace not zero
        if (this->distance(node) != 0 && !node.visited) {

            if (closest_node == nullptr || this->distance(node) < this->distance(*closest_node)) {
                closest_node = &parent_model->SNodes()[node_index];
            }
        }
    }
	//update
    return closest_node;
}

//define empty function 
void RouteModel::Node::FindNeighbors() {

	//for each road ref in vector 
    for (auto & road : parent_model->node_to_road[this->index]) {

		//should use the function to create pointer
        RouteModel::Node *new_neighbor = this->FindNeighbor(parent_model->Ways()[road->way].nodes);

		//if that pointer not nullpttr push the pointerto back of 
        if (new_neighbor) {

            this->neighbors.emplace_back(new_neighbor);
        }
    }
}

//Add public class accept 2 float vlaue as argument
RouteModel::Node &RouteModel::FindClosestNode(float x, float y) {
	//create temporary node with x,y coordinates
    Node input;
    input.x = x;
    input.y = y;
	//create float for min distace found in your search 
    float min_dist = std::numeric_limits<float>::max();

    float dist;
	//create int value to store the index of the closest
    int closest_idx;
	
	//loop that iterate through the vector given by Road()
	//for each ref road in vector check:
    for (const Model::Road &road : Roads()) {

		//type is not footway..if the road not footway
        if (road.type != Model::Road::Type::Footway) {

			//loop over each node index in the way that road belong to
            for (int node_idx : Ways()[road.way].nodes) {
                dist = input.distance(SNodes()[node_idx]);
                if (dist < min_dist) {
					//update 
                    closest_idx = node_idx;
                    min_dist = dist;
                }
            }
        }
    }

    return SNodes()[closest_idx];
}
