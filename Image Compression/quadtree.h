
/**
 *
 * quadtree (pa3)
 *
 */

#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include <cmath>
#include <utility>

#include "cs221util/PNG.h"
#include "cs221util/RGBAPixel.h"
#include "stats.h"
using namespace std;
using namespace cs221util;

/**
 * quadtree: This is a structure used in decomposing an image
 * into squares of similarly colored pixels.
 *
 * You should not remove anything from this class definition, but
 * you will find it helpful to add your own private helper functions to it.
 *
 * This file will be used for grading.
 */

class quadtree {
protected:
    /**
     * The Node class is private to the tree class via the principle of
     * encapsulation---the end user does not need to know our node-based
     * implementation details. (You may find it useful to make this public
     * for debugging.
     *
     * given for PA3
     */
    class Node {
    public:
        Node(pair<int, int> ul, int d, RGBAPixel a, double v);  // Node constructor
        pair<int, int> upLeft;
        int dim;
        RGBAPixel avg;
        double var;
        Node* NW;  // ptr to NW subtree
        Node* NE;  // ptr to NE subtree
        Node* SE;  // ptr to SE subtree
        Node* SW;  // ptr to SW subtree
    };

public:
    /* =============== start of given functions ====================*/

    /**
     * Copy constructor for a quadtree. GIVEN
     * Since quadtree allocate dynamic memory (i.e., they use "new", we
     * must define the Big Three). This depends on your implementation
     * of the copy funtion.
     * @see quadtree.cpp
     *
     * @param other The quadtree we are copying.
     */
    quadtree(const quadtree& other);

    /**
     * quadtree destructor.
     * Destroys all of the memory associated with the
     * current quadtree. This function should ensure that
     * memory does not leak on destruction of a quadtree.
     *
     * @see quadtree.cpp
     */
    ~quadtree();  // allows for derived class destructor

    /**
     * Overloaded assignment operator for quadtree.
     * Part of the Big Three that we must define because the class
     * allocates dynamic memory. This depends on your implementation
     * of the copy and clear funtions.
     * @see quadtree.cpp
     *
     * @param rhs The right hand side of the assignment statement.
     */
    quadtree& operator=(const quadtree& rhs);

    /* =============== end of given functions ====================*/

    /* =============== public PA3 FUNCTIONS =========================*/

    /**
     * Constructor that builds a quadtree out of the given PNG.
     * Every leaf in the tree corresponds to a pixel in the PNG.
     * Every non-leaf node corresponds to a 2^k x 2^k square of pixels
     * in the original PNG, represented by an (x,y) pair for the
     * upper left corner of the square and an integer dimension, k.
     * In addition, the Node stores a pixel representing the average
     * color over the square. It also stores a measure of color
     * variability over the square.
     *
     * Every node's children correspond to a partition
     * of the node's square into four smaller squares. The
     * current node's square is split in half horizontally
     * and vertically to produce the 4 children.
     *
     * This function will also build the stats object used to compute
     * average pixel color and variability, over the squares.
     */
    quadtree(PNG& imIn);

    /**
     * Render returns a PNG image consisting of the pixels
     * stored in the tree. It may be used on pruned trees. Draws
     * every leaf node's square onto a PNG canvas using the
     * average color stored in the node.
     */
    PNG render() const;

    /**
     *  Prune function trims subtrees as high as possible in the tree.
     *  A subtree is pruned (cleared) if prunable returns true.
     *  Pruning criteria should be evaluated on the original tree, not
     *  on a pruned subtree. (we only expect that trees would be pruned once.)
     *
     * You may want a recursive helper function for this one.
     */
    void prune(const int tol);

    /*
     * The pruneSize function takes a tolerance as input, and returns
     * the number of leaves that would result if the tree were to
     * be pruned with that tolerance. Consistent with the definition
     * of prune, a node is counted if prunable returns true.
     *
     * You may want a recursive helper function for this one.
     */
    int pruneSize(const int tol) const;

    /*
     * The idealPrune function can be considered to be the inverse
     * of the pruneSize function. It takes as input a number of leaves
     * and returns the minimum tolerance that would produce that resolution
     * upon a prune. It does not change the structure of the tree.
     */
    int idealPrune(const int leaves) const;

    /* =============== end of public PA3 FUNCTIONS =========================*/

private:
    /*
     * Private member variables.
     *
     * You must use these as specified in the spec and may not rename them.
     * You may add more if you need them.
     */

    Node* root;  // ptr to the root of the quadtree

    int edge;  // side length of the square image

    /* =================== start of private PA3 functions ============== */

    /**
     * Destroys all dynamically allocated memory associated with the
     * current quadtree class.
     * You may want a recursive helper function for this one.
     */
    void clear();

    /**
     * Copies the parameter other quadtree into the current quadtree.
     * Does not free any memory. Called by copy constructor and op=.
     * You may want a recursive helper function for this one.
     * @param other The quadtree to be copied.
     */
    void copy(const quadtree& other);

    /**
     * Private helper function for the constructor. Recursively builds
     * the tree according to the specification of the constructor.
     * @param s Contains the data used to calc avg and var
     * @param ul upper left point of current node's square.
     * @param dim reflects the size of the current square
     */
    Node* buildTree(stats& s, pair<int, int> ul, int dim);

    /* prunable is a pure virtual function, and as such it must
     * be implemented in a derived class. Prunable takes parameters
     * node, and tol, and returns true if node can be pruned (its
     * subtrees cleared) according to the criteria specified in
     * the derived class.
     */
    virtual bool prunable(Node* node, const int tol) const = 0;

    /* =================== end of private PA3 functions given ============== */

    void render(Node* subtree, PNG & png) const; 
    void prune(Node*& subtree, const int tol);
    int pruneSize(Node* subtree, const int tol, int & currNum) const;
    void clear(Node* & subtree);
    //int idealPrune(const int leaves, int & tol, int & inc) const; // attempt 1
    //int idealPrune(const int minL, const int maxL, int & tol, int & inc) const; // attempt 2
    Node* copy(Node* subtree);
};

#endif
