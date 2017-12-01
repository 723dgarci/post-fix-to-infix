//
// Created by WorkD on 10/31/2017.
//

#ifndef ASSIGNMENT4_BET_H
#define ASSIGNMENT4_BET_H

#include <iostream>
#include <string>
#include <list>
#include <sstream>
#include <stack>
#include <queue>
class BET {
public:


    BET();                                              // default zero-parameter constructor. Builds an empty tree.
    BET(const std::string& postfix);                    // one-parameter constructor, where parameter "postfix" is string containing a postfix expression. The tree should be built based on the postfix expression. Tokens in the postfix expression are separated by spaces.
    BET(const BET&);                                    // copy constructor -- makes appropriate deep copy of the tree
    ~BET();                                             // destructor -- cleans up all dynamic space in the tree
    bool buildFromPostfix(const std::string& postfix);  // parameter "postfix" is string containing a postfix expression. The tree should be built based on the postfix expression. Tokens in the postfix expression are separated by spaces. If the tree contains nodes before the function is called, you need to first delete the existing nodes. Return true if the new tree is built successfully. Return false if an error is encountered.
    const BET & operator= (const BET &);                // assignment operator -- makes appropriate deep copy

    void printInfixExpression();                        // Print out the infix expression. Should do this by making use of the private (recursive) version

    void printPostfixExpression();                      // Print the postfix form of the expression. Use the private recursive function to help
    size_t size();                                      // Return the number of nodes in the tree (using the private recursive function)
    size_t leaf_nodes();                                // Return the number of leaf nodes in the tree. (Use the private recursive function to help)
    bool empty();                                       // return true if the tree is empty. Return false otherwise.
    void levelorder();



private:
    struct BinaryNode{
        std::string element;
        BinaryNode * left;
        BinaryNode * right;
        BinaryNode(const std::string & e, BinaryNode* l = nullptr, BinaryNode*r = nullptr)
                :element{e},left{l},right{r}{}
        BinaryNode(std::string && e, BinaryNode* l= nullptr, BinaryNode*r = nullptr)
                :element{std::move(e)},left{l},right{r}{}
    };
    BinaryNode* root;

    // Private helper functions (all the required private member functions must be implemented recursively):
    void printInfixExpression(BinaryNode *n);           // print to the standard output the corresponding infix expression. Note that you may need to add parentheses depending on the precedence of operators. You should not have unnecessary parentheses.
    void makeEmpty(BinaryNode* &t);                     // delete all nodes in the subtree pointed to by t.
    BinaryNode * clone(BinaryNode *t);                  // clone all nodes in the subtree pointed to by t. Can be called by functions such as the assignment operator=.
    void printPostfixExpression(BinaryNode *n);         // print to the standard output the corresponding postfix expression.
    size_t size(BinaryNode *t);                         // return the number of nodes in the subtree pointed to by t.
    size_t leaf_nodes(BinaryNode *t);                   // return the number of leaf nodes in the subtree pointed to by t.


    //my helper functions
    void insert(BinaryNode *& t,BinaryNode* l =nullptr, BinaryNode*r = nullptr);
    BinaryNode * newBN(const std::string &e, BinaryNode* l = nullptr, BinaryNode * r= nullptr){ return new BinaryNode(e,l,r);}
    BinaryNode * newBN( std::string && e, BinaryNode* l= nullptr, BinaryNode * r= nullptr){ return new BinaryNode(e,l,r);}
    bool asmd(const std::string &) const;
    bool isParent(BinaryNode * t) const;
    int pres(const std::string& s);
};

    ///additional funcitons
    void BET::levelorder () {
        if(!empty()) {
            std::queue<BinaryNode *> q;
            q.push(root);
            while (!q.empty()) {
                auto n = q.front();
                if(n->left != nullptr)
                    q.push(n->left);
                if(n->right != nullptr)
                    q.push(n->right);
                std::cout << n->element << ' ';
                q.pop();
            }
            std::cout << "\n";
        }
    }

    ///given functions to define
    BET::BET():root(nullptr) {}                          // default zero-parameter constructor. Builds an empty tree.



    BET::BET(const std::string& postfix) {               // one-parameter constructor, where parameter "postfix" is string containing a postfix expression. The tree should be built based on the postfix expression. Tokens in the postfix expression are separated by spaces.
        buildFromPostfix(postfix);
    }


    BET::BET(const BET& rhs): root(nullptr){                // copy constructor -- makes appropriate deep copy of the tree
        root = clone(rhs.root);
    }

    BET::~BET() {                                           // destructor -- cleans up all dynamic space in the tree
       makeEmpty(root);
    }

    bool  BET::buildFromPostfix(const std::string& postfix) {       // parameter "postfix" is string containing a postfix expression. The tree should be built based on the postfix expression. Tokens in the postfix expression are separated by spaces. If the tree contains nodes before the function is called, you need to first delete the existing nodes. Return true if the new tree is built successfully. Return false if an error is encountered.
        // need to do syntaz checking - this will return false
        std::stringstream line(postfix);
        std::string str;
        std::list<BinaryNode*> Q;
        int count = 0;
        //put string into queue
        while(line >> str) {
            if(asmd(str)){                  //get difference of number of operators and operands
                count--;
            }else{
                count++;
            }
            Q.push_back(newBN(str));

        }
        if(count != 1){                     //not the right number or operators and/or operand
            std::cout << "Wrong postfix expression\n";
            return false;
        }
        if(Q.size() == 1){
            root = Q.front();
            return true;
        }

        std::stack<BinaryNode*> store;
        if(asmd(Q.front()->element)){       //if the first 2 elements are operators then it is not a valid post fix equation
            std::cout << "Wrong postfix expression\n";
            return false;
        }

        store.push(Q.front());
        Q.pop_front();
        if(asmd(Q.front()->element)){
            std::cout << "Wrong postfix expression\n";
            return false;
        }
        store.push(Q.front());
        Q.pop_front();

        if(!asmd(Q.back()->element)){                   //last element is a operand
            std::cout << "Wrong postfix expression\n";
            return false;
        }


        while(!Q.empty()){                              // it is an operator
            if(asmd(Q.front()->element)){
                Q.front()->right =  store.top();
                store.pop();
                Q.front()->left = store.top();
                store.pop();
                store.push(Q.front());
            }
            else{                                       //it is an operand
                store.push(Q.front());
            }
            root = store.top();
            Q.pop_front();
        }
        return true;
    }

   const BET & BET::operator= (const BET & B) {                // assignment operator -- makes appropriate deep copy
        if(&B != this){
            makeEmpty(root);
            root = clone(B.root);
        }
       return *this;
   }
    void  BET::printInfixExpression() {                        // Print out the infix expression. Should do this by making use of the private (recursive) version
        printInfixExpression(root);
        std::cout <<std::endl;
    }


    void  BET::printPostfixExpression() {                      //: Print the postfix form of the expression. Use the private recursive function to help
        printPostfixExpression(root);
        std::cout <<std::endl;
    }


    size_t BET::size() {                                      // Return the number of nodes in the tree (using the private recursive function)
        return  size(root);
    }

    size_t BET::leaf_nodes(){ return leaf_nodes(root);}                                //: Return the number of leaf nodes in the tree. (Use the private recursive function to help)


    bool  BET::empty() { return root == nullptr;}                                       //: return true if the tree is empty. Return false otherwise.


// Private helper functions (all the required private member functions must be implemented recursively):


    void BET::printInfixExpression(BET::BinaryNode *n) {           // print to the standard output the corresponding infix expression. Note that you may need to add parentheses depending on the precedence of operators. You should not have unnecessary parentheses.

        if(n != nullptr){
            if(isParent(n)&& isParent(n->left)) {
                if (pres(n->left->element) < pres(n->element) ) {
                    std::cout << "( ";
                }
            }
            printInfixExpression(n->left);
            if(isParent(n)&& isParent(n->left)) {
                if (pres(n->left->element) < pres(n->element) ) {
                    std::cout << ") ";
                }
            }

            std::cout<<n->element<< ' ';

            if(isParent(n) && isParent(n->right)) {
                if (pres(n->right->element) <= pres(n->element)) {
                    std::cout << "( ";
                }
            }
            printInfixExpression(n->right);
            if(isParent(n) && isParent(n->right)) {
                if (pres(n->right->element) <= pres(n->element)) {
                    std::cout << ") ";
                }
            }
        }
    }


    void BET::makeEmpty(BinaryNode* &t) {                     // delete all nodes in the subtree pointed to by t.
        if( t != nullptr){
            makeEmpty(t->left);
            makeEmpty(t->right);
            delete t;
            t = nullptr;
        }
    }

    BET::BinaryNode *  BET::clone(BET::BinaryNode *t) {                  // clone all nodes in the subtree pointed to by t. Can be called by functions such as the assignment operator=.
        if(t == nullptr){
            return nullptr;
        }else{
            return new BET::BinaryNode{t->element,clone(t->left), clone(t->right)};
        }
    }


    void  BET::printPostfixExpression(BinaryNode *n) {         // print to the standard output the corresponding postfix expression.
        if(n != nullptr){
            printPostfixExpression(n->left);
            printPostfixExpression(n->right);
            std::cout << n->element<< ' ';
        }
    }


    size_t BET::size(BinaryNode *t) {                         // return the number of nodes in the subtree pointed to by t.
        if(t == nullptr){
            return 0;
        }
        else{
            return size(t->left)+ size(t->right) +1;
        }
    }

    size_t  BET::leaf_nodes(BinaryNode *t) {                   // return the number of leaf nodes in the subtree pointed to by t.
        if(t == nullptr){
            return 0;
        }else if(isParent(t)){
            return leaf_nodes(t->left) + leaf_nodes(t->right);
        }else
            return 1;
    }

    void BET::insert(BET::BinaryNode *& t ,BET::BinaryNode* l, BET::BinaryNode * r){
        t->left = l;
        t->right = r;
    }

    bool BET::asmd(const std::string & s)const {return s == "+"|| s =="-" ||s == "/"||s == "*";}

    int BET::pres(const std::string& s){
        if(s == "*" || s =="/"){
            return 3;
        } else if(s == "+" || s =="-"){
            return 2;
        }else {
            return 1;
        }
    }

    bool BET::isParent(BET::BinaryNode * t) const{
        return t->left != nullptr && t->right!= nullptr;
    }
#endif //ASSIGNMENT4_BET_H