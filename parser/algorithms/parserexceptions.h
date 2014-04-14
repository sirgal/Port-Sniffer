#ifndef PARSEREXCEPTIONS_H
#define PARSEREXCEPTIONS_H

#include <exception>

struct ExceptionWithPos : public std::exception
{
    int position;
    ExceptionWithPos(int position) : position(position) { }

    virtual const char* what() const throw() = 0;
};

struct InvalidCodeFormat : public ExceptionWithPos
{
    InvalidCodeFormat(int position) : ExceptionWithPos(position) { }

    virtual const char* what() const throw()
    {
        return "Invalid byte format";
    }
};

struct UnknownActionType : public ExceptionWithPos
{
    UnknownActionType(int position) : ExceptionWithPos(position) { }

    virtual const char* what() const throw()
    {
        return "Unsupported action type";
    }
};

struct NonNumericParam : public ExceptionWithPos
{
    NonNumericParam( int fail_pos ) : ExceptionWithPos(fail_pos) { }

    virtual const char* what() const throw()
    {
        return "Parameter is expected to be a number";
    }
};

struct InvalidComment : public ExceptionWithPos
{
    InvalidComment(int comment_start) : ExceptionWithPos(comment_start) { }

    virtual const char* what() const throw()
    {
        return "Invalid comment found";
    }
};

struct InfiniteComment : public InvalidComment
{
    InfiniteComment(int comment_start) : InvalidComment(comment_start) { }

    virtual const char* what() const throw()
    {
        return "Comment never ends";
    }
};

struct SyntaxError : public ExceptionWithPos
{
    char unexpected_symbol;
    SyntaxError( char unexpected_symbol, int pos )
        : ExceptionWithPos(pos), unexpected_symbol(unexpected_symbol)
    { }

    virtual const char* what() const throw()
    {
        return "Syntax error found";
    }
};

struct StrongViolation : public ExceptionWithPos
{
    StrongViolation(int position) : ExceptionWithPos(position) { }

    virtual const char* what() const throw()
    {
        return "Mix of strong and weak statements found";
    }
};

struct AnyChannelViolation : public ExceptionWithPos
{
    AnyChannelViolation(int position) : ExceptionWithPos(position) { }

    virtual const char* what() const throw()
    {
        return "Use of any channel action with any other actions found";
    }
};

struct IncompatibleActions : public ExceptionWithPos
{
    IncompatibleActions(int position) : ExceptionWithPos(position) { }

    virtual const char* what() const throw()
    {
        return "Incompatible actions on the same channel";
    }
};

struct CycleMixing : public ExceptionWithPos
{
    CycleMixing( int position ) : ExceptionWithPos(position) { }

    virtual const char* what() const throw()
    {
        return "Mixing cycle with anything is not allowed";
    }
};

#endif // PARSEREXCEPTIONS_H
