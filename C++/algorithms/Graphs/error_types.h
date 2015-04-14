#ifndef ERROR_TYPES_H
#define ERROR_TYPES_H

#include <string>
#include <sstream>
#include <exception>

class BadVertexException : public std::exception {
public:
    BadVertexException(std::string _name)
        : name(_name) { }
    ~BadVertexException() throw() { }
    virtual const char* what() const throw() {
        return ("Accessing vertex: \"" + name + "\" failed!").c_str();
    }

    std::string name;
};

class BadEdgeException : public std::exception {
public:
    BadEdgeException(std::string _source, std::string _sink)
        : source(_source), sink(_sink) { }
    ~BadEdgeException() throw() { }
    virtual const char* what() const throw() {
        return ("Adding edge: \"" + source + " -> " + sink + "\" failed!").c_str();
    }

    std::string source;
    std::string sink;
};

class EdgeReassignmentException : public std::exception {
public:
    EdgeReassignmentException(std::string _source, std::string _sink, int oldW, int newW)
        : source(_source), sink(_sink), oldWeight(oldW), newWeight(newW) { }
    ~EdgeReassignmentException() throw() { }
    virtual const char* what() const throw() {
        std::stringstream output;
        output << "Edge redefinition detected: "
               << "\"" << source << " -> " << sink << "\"! "
               << "Old weight: " << oldWeight << ". "
               << "New Weight: " << newWeight << ".";
        return output.str().c_str();
    }

    std::string source;
    std::string sink;
    int oldWeight;
    int newWeight;
};

class UndirectedGraphEdgeException : public std::exception {
public:
    UndirectedGraphEdgeException(std::string _source, std::string _sink, int oldW, int newW)
        : source(_source), sink(_sink), oldWeight(oldW), newWeight(newW) { }
    ~UndirectedGraphEdgeException() throw() { }
    virtual const char* what() const throw() {
        std::stringstream output;
        output << "Unsupported imbalance in undirected edges from: "
               << "\"" << source << " <-> " << sink << "\"! "
               << "Old weight: " << oldWeight << ". "
               << "New Weight: " << newWeight << ".";
        return output.str().c_str();
    }

    std::string source;
    std::string sink;
    int oldWeight;
    int newWeight;
};

#endif
