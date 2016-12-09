#include <iostream>
#include <fstream>
#include <sstream>
#include <climits>
#include "Queue.h"
using namespace std;

static int INFINITY = INT_MAX;
static int NOEDGE = -1;
static int cityCount = 0;

struct City
{
	string name;
	int count;
	string * adj;
	int * costs;
};

City * newCity(string name)
{
	City * c = new City;
	c->name = name;
	c->count = 0;
	c->adj = new string[4];
	c->costs = new int[4];
	return c;
}

int indexOf(string * arr, string s)
{
	for(int x = 0; x < cityCount; x++)
	{
		if(arr[x].compare(s) == 0)
		{
			return x;
		}
	}
	return -1;
}

int * newIntNoEdges(int size)
{
	int * arr = new int[size];
	for( int x = 0; x < size; x++) arr[x] = NOEDGE;
	return arr;
}

int * newIntInfititeCosts(int size)
{
	int * arr = new int[size];
	for( int x = 0; x < size; x++) arr[x] = INFINITY;
	return arr;
}

int ** new2DIntArray(int size)
{
	int ** arr = new int*[size];
	
	for(int x = 0; x < size; x++)
	{
		arr[x] = newIntNoEdges(size);
	}
	
	return arr;
}

int * dijkstras(int ** costFrom, int start, int end,string * names)
{
	int * cost_from_start_to = newIntInfititeCosts(cityCount);
	int * best_parent = newIntInfititeCosts(cityCount); 
	bool * visited = new bool[cityCount];

	Queue<int> * queue = new Queue<int>();
	queue->enqueue(&start);
	best_parent[start] = start;
	cost_from_start_to[start] = 0;

	while(!queue->empty())
	{
		int currentCity = *queue->dequeue();
		
		for(int neighbor = 0; neighbor < cityCount; neighbor++)
		{
			if(costFrom[currentCity][neighbor] != NOEDGE)
			{
				int newCostToNeighbor = cost_from_start_to[currentCity] + costFrom[currentCity][neighbor];
				if(newCostToNeighbor < cost_from_start_to[end] && newCostToNeighbor < cost_from_start_to[neighbor]) 
				{
					cost_from_start_to[neighbor] = newCostToNeighbor;
					best_parent[neighbor] = currentCity;
					queue->enqueue(new int(neighbor));
				}
			}
		}

		visited[currentCity] = true;

	}

	return best_parent;
}

void getPath(Queue<int> * path, int * indexes, int stop)
{
	if(indexes[stop] == stop) 
	{
		path->enqueue(new int(stop));
		return;
	}
	getPath(path,indexes,indexes[stop]);
	path->enqueue(new int(stop));
}

void printPath(Queue<int> * q, string * names)
{
	for(Node<int> * current = q->front; current; current = current->next)
	{
		cout << names[*current->value]<< endl;
	}
}

string * splitByTab(string s)
{
	int count = 0;
	
	for(int x = 0; x < s.length(); x++)
	{
		if(s.at(x) == '\t')
		{
			count++;
		}
	}
	
	stringstream ss(s);
	string * arr = new string[count+1];
	
	for(int x = 0; x < count+1; x++)
	{
		ss >> arr[x];
	}
	
	return arr;
}

void populateCosts(int ** costFrom, Node<City> * front, string * names)
{
	Node<City> * current = front;
	
	for(int x = 0; x < cityCount; x++)
	{
		City * c = current->value;
		
		string city1 = c->name;
		int index1 = indexOf(names,city1);

		for(int y = 0; y < c->count; y++)
		{
			string city2 = c->adj[y];
			int index2 = indexOf(names,city2);

			costFrom[index1][index2] = c->costs[y];
		}

		current = current->next;
	}
}

int ** deleteRows(int ** oldCosts, Queue<int> * removeRows)
{
	int ** newCosts = new int*[cityCount];

	for(int x = 0; x < cityCount; x++)
	{
		int * row = new int[cityCount];

		for(int y = 0; y < cityCount; y++)
		{
			if(removeRows->contains(new int(y)))
			{
				row[y] = NOEDGE;
			}
			else
			{
				row[y] = oldCosts[x][y];
			}
		}

		newCosts[x] = row;
	}
	
	return newCosts;
}

Queue<City>* input(ifstream * file, string * line)
{
	Queue<City> * cities = new Queue<City>();

	while(getline(*file,*line))
	{
		if(line->length() != 0 && line->at(0) != '#')
		{
			if(line->substr(0,4).compare("trip") == 0)
			{
				break;
			}
			else
			{
				string * split = splitByTab(*line);
				
				int cost = stoi(split[2]);
				
				City * source = cities->find(split[0]);
				if(!source)
				{
					source = newCity(split[0]);
					cities->enqueue(source);
				}

				City * destination = cities->find(split[1]);
				if(!destination)
				{
					destination = newCity(split[1]);
					cities->enqueue(destination);
				}

				source->adj[source->count] = split[1];
				source->costs[source->count] = cost;
				source->count++;

				destination->adj[destination->count] = split[0];
				destination->costs[destination->count] = cost;
				destination->count++;
			}
		}
	}
	
	return cities;
}

int main(int argc, char ** argv)
{
	if(argc < 2) return -1;
	string filename = argv[1];	
	filename = filename.substr(6);
	ifstream file(filename);
	string line = "";
	Queue<City> * cities = input(&file, &line);

	cityCount = cities->count;
	string * names = new string[cityCount];

	Node<City> * current = cities->front;

	for(int x = 0; x < cityCount; x++)
	{
		names[x] = current->value->name;
		current = current->next;
	}	
	
	int ** costFrom = new2DIntArray(cityCount);
	populateCosts(costFrom, cities->front, names);

	do 
	{
		string * split = splitByTab(line);
		int startIndex = indexOf(names,split[1]);
		int stopIndex = indexOf(names,split[2]);

		int * indexes = dijkstras(costFrom,startIndex,stopIndex,names);
		
		Queue<int> * path = new Queue<int>();
		getPath(path,indexes,stopIndex);
		printPath(path,names);
		
		path->dequeue();
		path->removeBack();
		int ** newCosts = deleteRows(costFrom, path);
		
		indexes = dijkstras(newCosts,stopIndex,startIndex,names);
		
		path = new Queue<int>();
		getPath(path,indexes,startIndex);
		printPath(path,names);
	}
	while(getline(file,line));
}
