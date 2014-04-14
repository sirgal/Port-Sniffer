#ifndef STREAMKMP_H
#define STREAMKMP_H

#include <vector>

// Knuth-Morris-Pratt substring search algorithm
// optimized for char-after-char use
class StreamKMP
{
    std::vector<char> substring;
    std::vector<int>  table;
    unsigned int m, i;
    bool search_done;

    void buildTable()
    {
        int position  = 2;
        int candidate = 0;
        int table_size = std::max((int) substring.size(), 2);

        table = std::vector<int>(table_size, 0);

        table[0] = -1;
        table[1] = 0;

        while( position < table_size ) {
            if( substring[position-1] == substring[candidate] ) {
                table[position++] = ++candidate;
            } else if( candidate > 0 ) {
                candidate = table[candidate];
            } else {
                table[position++] = 0;
            }
        }
    }
public:
    StreamKMP( std::vector<char> substring )
        : substring(substring), search_done(false)
    {
        reset();
        buildTable();
    }

    void reset()
    {
        m = i = 0;
        search_done = false;
    }

    //true: substring found; false: substring not found yet;
    bool nextChar( char input )
    {
        if( search_done )
            return true;

        if( substring[i] == input ) {
            i++;
        } else {
            m += i - table[i];

            if( table[i] > -1 )
                i = table[i];
            else
                i = 0;
        }

        if( i == substring.size() ) {
            search_done = true;
            return true;
        } else {
            return false;
        }
    }

    bool isDone()
    {
        return search_done;
    }

};

#endif // STREAMKMP_H
