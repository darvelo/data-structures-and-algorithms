#ifndef UTILS_H
#define UTILS_H

#include <exception>

class CustomException : public std::exception {
public:
    CustomException(const char* _message)
        : message(_message) { }
    ~CustomException() throw() { }
    virtual const char* what() const throw() {
        return message;
    }
protected:
    const char* message;
};

#endif
