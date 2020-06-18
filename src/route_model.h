#ifndef ROUTE_MODEL_H
#define ROUTE_MODEL_H

#include <limits>
#include <cmath>
#include <unordered_map>
#include "model.h"
#include <iostream>

class RouteModel : public Model {

  public:
    //class contain all data needed to perform A* 
    class Node : public Model::Node {
      public:
	//
        Node * parent = nullptr;

        //h-value for node "inital to max possible"
        float h_value = std::numeric_limits<float>::max();

	//g-value for node "inital for 0.0"        
	float g_value = 0.0;

	//to indicate if node has been visited "flase"
        bool visited = false;

	//vector of node pointer named "neighbors"
        std::vector<Node *> neighbors;

	//Write distance Function "find distance between 2 nodes this will allow to find closed node to current node"
        void FindNeighbors();

	//declear const distance with return float
        float distance(Node other) const {

		//return eucliden distance from current nodeto node passed            
		return std::sqrt(std::pow((x - other.x), 2) + std::pow((y - other.y), 2));
        }

        Node(){}
	
        Node(int idx, RouteModel * search_model, Model::Node node) : Model::Node(node), parent_model(search_model), index(idx) {}

      private:
        int index;
	//should accept a vector argument and return pointer to node
        Node * FindNeighbor(std::vector<int> node_indices);

	//pointer to RouteModel object named parent_model "allow each node to access data stored in parent model"
        RouteModel * parent_model = nullptr;
    };

    RouteModel(const std::vector<std::byte> &xml);
    Node &FindClosestNode(float x, float y);

    //return a refrance to vector of SNode stored as m-Node
    auto &SNodes() { return m_Nodes; }
    std::vector<Node> path;
    
  private:
    //create function to search path to follow road that node are on not between of node
    void CreateNodeToRoadHashmap();

    //a hash table of node index value to vector of road pointer this value should be "unroad_map with key type "int", vlaue "vector of const model::Road" "
    std::unordered_map<int, std::vector<const Model::Road *>> node_to_road;

    //Store all node from open street map data
    std::vector<Node> m_Nodes;

};

#endif
