

/******************************************************************************
 * File: Trailblazer.cpp
 *
 * Implementation of the graph algorithms that comprise the Trailblazer
 * assignment.
 */

#include "Trailblazer.h"
#include "TrailblazerGraphics.h"
#include "TrailblazerTypes.h"
#include "TrailblazerPQueue.h"
#include "vector.h"
#include "map.h"
#include "random.h"
#include "set.h"
#include "foreach.h"
using namespace std;

/* Function: shortestPath
 * 
 * Finds the shortest path between the locations given by start and end in the
 * specified world.	 The cost of moving from one edge to the next is specified
 * by the given cost function.	The resulting path is then returned as a
 * Vector<Loc> containing the locations to visit in the order in which they
 * would be visited.	If no path is found, this function should report an
 * error.
 *
 * In Part Two of this assignment, you will need to add an additional parameter
 * to this function that represents the heuristic to use while performing the
 * search.  Make sure to update both this implementation prototype and the
 * function prototype in Trailblazer.h.
 */
Vector<Loc>
shortestPath(Loc start,
             Loc end,
             Grid<double>& world,
             double costFn(Loc from, Loc to, Grid<double>& world),
			 double h(Loc from, Loc to, Grid<double>& world)) {
	// TODO: Fill this in!
	TrailblazerPQueue<Loc> pqueue;
	Set<Loc> yellows;
	Set<Loc> greens;
	pqueue.enqueue(start, h(start, end, world));
	Map<Loc, Loc> parents;
	Map<Loc, double> distances;
	colorCell(world, start, YELLOW);
	yellows.add(start);
	distances.put(start, 0);
	while(!pqueue.isEmpty()) {
		Loc curr = pqueue.dequeueMin();
		colorCell(world, curr, GREEN);
		greens.add(curr);
		if(curr == end) {
			break;
		}
		for(int i = -1; i <= 1; i++) {
			for(int j = -1; j <= 1; j++) {
				if(world.inBounds(curr.row + i, curr.col + j) && !(i == 0 && j == 0)) {
					int nextRow = curr.row + i;
					int nextCol = curr.col + j;
					Loc v = makeLoc(nextRow, nextCol);
					double newDistance = distances[curr] + costFn(curr, v, world);
					if(!yellows.contains(v) && !greens.contains(v)) {
						colorCell(world, v, YELLOW);
						yellows.add(v);
						distances[v] = newDistance;
						parents[v] = curr;
						pqueue.enqueue(v, newDistance + h(v, end, world));
					}
					else if(yellows.contains(v) && distances[v] > newDistance) {
						pqueue.decreaseKey(v, newDistance + h(v, end, world));
						distances[v] = newDistance;
						parents[v] = curr;
					}
				}
			}
		}
	}
	Vector<Loc> path;
	Loc curr = end;
	while(curr != start) {
		path.add(curr);
		curr = parents[curr];
	}	
	path.add(start);
	int i = 0;
	int j = path.size() - 1;
	while (i < j) {
		Loc temp = path[i];
		path[i] = path[j];
		path[j] = temp;
		i++;
		j--;
	}
    return path;
}

Set<Edge> createMaze(int numRows, int numCols) {
	// TODO: Fill this in!
	Set<Edge> maze;
	TrailblazerPQueue<Edge> pqueue;
	Map<Loc, int> clusters;
	for(int i = 0; i < numRows; i++) {
		for(int j = 0; j < numCols; j++) {
			Loc loc1 = makeLoc(i, j);
			clusters[loc1] = clusters.size();
			if(i > 0) {
				Loc loc2 = makeLoc(i - 1, j);
				Edge edge = makeEdge(loc1, loc2);
				pqueue.enqueue(edge, randomReal(0, 1));
			}
			if(j > 0) {
				Loc loc2 = makeLoc(i, j - 1);
				Edge edge = makeEdge(loc1, loc2);
				pqueue.enqueue(edge, randomReal(0, 1));
			}
		}
	}
	while(clusters.size() > 1 && !pqueue.isEmpty()) {
		Edge edge = pqueue.dequeueMin();
		int cluster1 = clusters[edge.start];
		int cluster2 = clusters[edge.end];
		if(cluster1 != cluster2) {
			foreach(Loc loc in clusters) {
				if(clusters[loc] == cluster1) {
					clusters[loc] = cluster2;
				}
			}
		maze.add(edge);
		}
	}
    return maze;
}
